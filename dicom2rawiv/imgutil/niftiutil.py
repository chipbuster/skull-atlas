#!/usr/bin/env python3

import numpy as np
import struct
import sys
import nibabel as nib
import pdb


def readNii(fname):
    """Read a given filename and return a dict"""

    img = nib.load(fname)

    if type(img) != nib.nifti1.Nifti1Image\
       and type(img) != nib.nifti1.Nifti2Image:
        raise ValueError(fname + " doesn't look like a NIFTI image.")

    retdict = {}

    # Copy header information into new dict
    for key in img.header:
        retdict[key] = img.header[key]

    # Force conversion from np.memmap to np.ndarray. This might choke weaker
    # machines, but should work for most of the CVC lab machines
    retdict['data'] = np.array(img.get_data())

    return retdict

def writeNii(data, fname, spacings):
    """Write the given numpy array to file."""

    assert np.ndim(data) == 3, "Need to have 3D data for writeNii"

    # Extract spacings and use them to generate the affine transform

    (xSpace,ySpace,zSpace) = spacings
    affineTransform = np.diag([xSpace,ySpace,zSpace,1]) # Stretch each dimension

    outImg = nib.Nifti1Image(data, affineTransform)

    outImg.to_filename(fname)

    return

def isNii(fname):
    """Determine if the file is a valid NIFTI file"""

    # Idea from https://brainder.org/2015/04/03/the-nifti-2-file-format
    # under section titled "NIFTI-1 or NIFTI-2?"
    niftiType = 0  # Encode "failure" as 0, and Nifti 1/2 as 1/2
    with open(fname,'rb') as infile:
        # Read first 4 bytes: value must be 348 for Nifti1 or 540 for Nifti2
        hdrSz = infile.read(4)

        # Try a little-endian read. Need either 348 or 540
        littleVal = struct.unpack("<i", hdrSz)[0]
        if littleVal == 348:
            niftiType = 1
        elif littleVal == 540:
            niftiType = 2
        else:
            # Try a big-endian read
            bigVal = struct.unpack(">i", hdrSz)[0]
            if bigVal == 348:
                niftiType = 1
            elif bigVal == 540:
                niftiType = 2
            else:
                niftiType = 0

        # If neither number matched, this is not a NIFTI
        if niftiType == 0:
            return niftiType

        # Check magic location to make sure this is a NIFTI and not a lucky hit
        if niftiType == 1:
            infile.seek(344)
            magicStr = infile.read(4)
            if magicStr == b"n+1\x00" or magicStr == b"ni1\x00":
                return niftiType #It matches!
            else:
                return 0         #This ain't no NIFTI

            if niftiType == 2:
                magicStr = infile.read(8)
                if magicStr == b"n+2\x00\x0d\x0a\x1a\x0a":
                    return niftiType      # Matches known NIFTI2 string
                else:
                    return 0              # Not a NIFTI
