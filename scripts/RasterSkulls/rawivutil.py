#!/usr/bin/env python3

# This script was copied from commit 87c13ba5cb5cb45100e483c5bfa4f50f8c4d2b2d
# I know this is bad practice. If you want to help, you can be the hero
# and refactor it (don't worry, it shouldn't be too hard!)

import numpy as np
import struct

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
    '''This function returns a dictionary with various entries, the last being the numpy array of data'''

    #These packers unpack the header bytes in big endian format
    floatpacker = struct.Struct('>f')
    intpacker = struct.Struct('>I')
    dataDict={}
    with open(fname,'rb') as infile:
        inp = infile.read(4)
        minX = floatpacker.unpack(inp)[0]
        dataDict['minX'] = minX

        inp = infile.read(4)
        minY = floatpacker.unpack(inp)[0]
        dataDict['minY'] = minY

        inp = infile.read(4)
        minZ = floatpacker.unpack(inp)[0]
        dataDict['minZ'] = minZ

        inp = infile.read(4)

        maxX = floatpacker.unpack(inp)[0]
        dataDict['maxX'] = maxX

        inp = infile.read(4)
        maxY = floatpacker.unpack(inp)[0]
        dataDict['maxY'] = maxY

        inp = infile.read(4)
        maxZ = floatpacker.unpack(inp)[0]
        dataDict['maxZ'] = maxZ

        inp = infile.read(4)
        numVerts = intpacker.unpack(inp)[0]
        dataDict['numVerts'] = numVerts

        inp = infile.read(4)
        numCells = intpacker.unpack(inp)[0]
        dataDict['numCells'] = numCells

        inp = infile.read(4)
        dimX = intpacker.unpack(inp)[0]
        dataDict['dimX'] = dimX

        inp = infile.read(4)
        dimY = intpacker.unpack(inp)[0]
        dataDict['dimY'] = dimY

        inp = infile.read(4)
        dimZ = intpacker.unpack(inp)[0]
        dataDict['dimZ'] = dimZ

        inp = infile.read(4)
        originX = floatpacker.unpack(inp)[0]
        dataDict['originX'] = originX

        inp = infile.read(4)
        originY = floatpacker.unpack(inp)[0]
        dataDict['originY'] = originY

        inp = infile.read(4)
        originZ = floatpacker.unpack(inp)[0]
        dataDict['originZ'] = originZ

        inp = infile.read(4)
        spanX = floatpacker.unpack(inp)[0]
        dataDict['spanX'] = spanX

        inp = infile.read(4)
        spanY = floatpacker.unpack(inp)[0]
        dataDict['spanY'] = spanY

        inp = infile.read(4)
        spanZ = floatpacker.unpack(inp)[0]
        dataDict['spanZ'] = spanZ

        #Now we unpack the data bytes in big endiand format and shape them in the
        #format specified by the header information (dimensions of x,y,z)
        inp=infile.read()
        arrpacker = struct.Struct('>%df' % numVerts)
        arrayData = arrpacker.unpack(inp)
        numpy_Arr=np.array(arrayData)
        numpy_Arr = np.reshape(numpy_Arr, (int(dataDict['dimX']),int(dataDict['dimY']),int(dataDict['dimZ'])), order= 'F')
        #print('the shape is',numpy_Arr.shape)
        dataDict['data']=numpy_Arr

        return(dataDict)

def replaceWithZeros(dictionary,lothreshold, hithreshold):

    '''This returns a dictionary with the data field filtered by replacing values lower than the lo threshold and higher than the hi threshold with zeros'''
    new_data=dictionary['data']
    new_data[new_data<int(lothreshold)]=np.zeros(1)
    new_data[new_data>int(hithreshold)]=np.zeros(1)

    dictionary['data']=new_data
    return(dictionary)
