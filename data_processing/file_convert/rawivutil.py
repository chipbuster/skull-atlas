#!/usr/bin/env python3

import numpy as np
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
    maxX = X * xgap + minX
    maxY = Y * ygap + minY
    maxZ = Z * zgap + minZ
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
