#!/usr/bin/env python3

import numpy as np
import struct
import sys
import nibabel as nib

def readNii(fname):
    """Read a given filename and return a dict"""

    img = nib.load(fname)

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
