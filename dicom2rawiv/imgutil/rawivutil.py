#!/usr/bin/env python3

import numpy as np
import os
import struct
import binascii

def writeRawIV(data, filename, spacings):
    """Writes an array to a rawiv file specified by filename."""

    assert isinstance(data, np.ndarray), "Data is not a NumPy array!"

    assert len(data.shape) == 3, "Data does not have 3 dimensions!"

    (X,Y,Z) = data.shape
    (xgap,ygap,zgap) = spacings

    ## Set up header information for rawiv files
    ## See https://svn.ices.utexas.edu/repos/cvc/trunk/VolumeRoverDoc/RoverManual.pdf for more info

    minX = 1.0
    minY = 1.0
    minZ = 1.0
    maxX = (X-1) * xgap + minX
    maxY = (Y-1) * ygap + minY
    maxZ = (Z-1) * zgap + minZ
    dimX = X
    dimY = Y
    dimZ = Z
    numVerts = dimX * dimY * dimZ
    numCells = (dimX - 1) * (dimY - 1) * (dimZ - 1)
    originX = 1.0
    originY = 1.0
    originZ = 1.0
    spanX = (maxX - minX)/(dimX - 1);
    spanY = (maxY - minY)/(dimY - 1);
    spanZ = (maxZ - minZ)/(dimZ - 1);

    header = bytearray()

    assert numVerts == data.size, "Something wrong with image dimension calculations"

    # If we don't create Struct objects, the struct module leaks memory
    # (Not technically a leak, but it behaves just like one)
    floatpacker = struct.Struct('>f')
    intpacker = struct.Struct('>I')
    arrpacker = struct.Struct('>%df' % data.size)

    # Pack the header info into a bytearray
    header.extend(floatpacker.pack(minX))  #Big-endian float
    header.extend(floatpacker.pack(minY))
    header.extend(floatpacker.pack(minZ))
    header.extend(floatpacker.pack(maxX))
    header.extend(floatpacker.pack(maxY))
    header.extend(floatpacker.pack(maxZ))
    header.extend(intpacker.pack(numVerts)) #Big-endian uint32
    header.extend(intpacker.pack(numCells))
    header.extend(intpacker.pack(dimX))
    header.extend(intpacker.pack(dimY))
    header.extend(intpacker.pack(dimZ))
    header.extend(floatpacker.pack(originX))  #Big-endian float
    header.extend(floatpacker.pack(originY))
    header.extend(floatpacker.pack(originZ))
    header.extend(floatpacker.pack(spanX))
    header.extend(floatpacker.pack(spanY))
    header.extend(floatpacker.pack(spanZ))

    assert len(header) == 68, "RawIV Header is wrong length, maybe this python platform uses nonstandard sizes?"

    with open(filename, 'wb',) as outfile:
        outfile.write(header)

        flatdata = data.ravel(order='F') #Column-major 1-D view of array

        packed = arrpacker.pack(*flatdata) #Pack whole array at once (faster!)
        outfile.write(packed)

    del floatpacker
    del intpacker
    del arrpacker

    return

def readRawIV(fname):
    '''Returns a dictionary with various entries representing a DICOM file'''

    #These packers unpack the header bytes in big endian format
    floatpacker = struct.Struct('>f')
    intpacker = struct.Struct('>I')
    rawivDict={}

    with open(fname,'rb') as infile:
        inp = infile.read(4)
        minX = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        minY = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        minZ = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        maxX = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        maxY = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        maxZ = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        numVerts = intpacker.unpack(inp)[0]

        inp = infile.read(4)
        numCells = intpacker.unpack(inp)[0]

        inp = infile.read(4)
        dimX = intpacker.unpack(inp)[0]

        inp = infile.read(4)
        dimY = intpacker.unpack(inp)[0]

        inp = infile.read(4)
        dimZ = intpacker.unpack(inp)[0]

        inp = infile.read(4)
        originX = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        originY = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        originZ = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        spanX = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        spanY = floatpacker.unpack(inp)[0]

        inp = infile.read(4)
        spanZ = floatpacker.unpack(inp)[0]

        # Place appropriate entries into rawiv representation
        rawivDict['min'] = (minX,minY,minZ)
        rawivDict['max'] = (maxX,maxY,maxZ)
        rawivDict['numVerts'] = numVerts
        rawivDict['numCells'] = numCells
        rawivDict['dim'] = (dimX,dimY,dimZ)
        rawivDict['origin'] = (originX,originY,originZ)
        rawivDict['span'] = (spanX, spanY, spanZ)

        # Calculate spacings as a convenience
        gapX = (maxX - minX) / (dimX - 1)
        gapY = (maxY - minY) / (dimY - 1)
        gapZ = (maxZ - minZ) / (dimZ - 1)

        rawivDict['spacings'] = (gapX, gapY, gapZ)

        # Now extract rawIV data and form into shape given by header data
        arrpacker = struct.Struct('>%df' % numVerts)
        inp=infile.read()

        arr = np.array(arrpacker.unpack(inp))
        imgdata = np.reshape(arr, (int(dimX),int(dimY),int(dimZ)), order= 'F')

        rawivDict['data'] = imgdata

        return rawivDict

def isRawIV(fname):
    """Determine if this is a rawiv file."""

    # This is tricky because there is no definitive standard for what qualifies as
    # a rawiv file. The best bet we have is to try to extract the dimensions and
    # see if they properly match the size of the file. If so, we can guess that
    # this is probably a RawIV. If not, either it's corrupted or it isn't.

    intpacker = struct.Struct('>I')
    rawivHeaderSize = 68

    with open(fname,'rb') as infile:
        infile.seek(32) #To start of dim

        rawX = infile.read(4)
        rawY = infile.read(4)
        rawZ = infile.read(4)

        dimX = intpacker.unpack(rawX)[0]
        dimY = intpacker.unpack(rawY)[0]
        dimZ = intpacker.unpack(rawZ)[0]

        # The correct size of the file is the data size plus the header size
        # The data can be 1, 2, or 4 bytes per element
        numElem = dimX * dimY * dimZ

        actualFileSize = os.path.getsize(fname)

        for dataElemSize in [1,2,4]:
            computedFileSize = numElem * dataElemSize + rawivHeaderSize
            if computedFileSize == actualFileSize:
                return True

        return False
