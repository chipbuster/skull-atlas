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

    # Pack the header info into a bytearray

    header.extend(struct.pack('>f',minX))  #Big-endian float
    header.extend(struct.pack('>f',minY))
    header.extend(struct.pack('>f',minZ))
    header.extend(struct.pack('>f',maxX))
    header.extend(struct.pack('>f',maxY))
    header.extend(struct.pack('>f',maxZ))
    header.extend(struct.pack('>I',numVerts)) #Big-endian uint32
    header.extend(struct.pack('>I',numCells))
    header.extend(struct.pack('>I',dimX))
    header.extend(struct.pack('>I',dimY))
    header.extend(struct.pack('>I',dimZ))
    header.extend(struct.pack('>f',originX))  #Big-endian float
    header.extend(struct.pack('>f',originY))
    header.extend(struct.pack('>f',originZ))
    header.extend(struct.pack('>f',spanX))
    header.extend(struct.pack('>f',spanY))
    header.extend(struct.pack('>f',spanZ))

    assert len(header) == 68, "RawIV Header is wrong length, maybe this python platform uses nonstandard sizes?"

    # Get flat view (not copy!) of data in column-major order as req'd
    # by the rawiv standard.
    flatdata = np.ravel(data, order='C')

    with open(filename, 'wb',) as outfile:
        outfile.write(header)

        # Machine epsilon at 2**12 is ~ 4e-4, so store as floats
        for elem in flatdata:
            outfile.write(struct.pack('>f',elem))

    return
