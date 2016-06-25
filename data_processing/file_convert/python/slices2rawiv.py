#!/usr/bin/env python3

import numpy as np
import dicom
import sys
import os
from rawivutil import *
from dicomutil import *
from collections import defaultdict

def read_all_dicom_in_dirpath(path):
    """Reads all the dicom images in a path into a set"""

    all_dicom = list()

    for fname in os.listdir(path):
        all_dicom.append(dicom.read_file(os.path.join(path,fname)))

    return all_dicom

def sep_into_bins(slices):
    """Given a group of DICOM slices, separate out into sets of DICOM images
    partitioned by the SeriesInstanceUID"""

    buckets = defaultdict(list)

    for img in slices:
        siuid = img.SeriesInstanceUID
        buckets[siuid].append(img)

    return buckets

def make_one_volimage(slices):
    """Takes a set of PyDICOM DataSet objects (returned by dicom.read_file) and
    renders them into a numpy array. This numpy array is ready to be written
    to file w/ writeRawIV. The function also returns the image spacings.
    The DataSet objects should all come from
    the same DICOM series: this is not enforced, and strange results may happen
    if this requirement is not met."""

    vol_image = np.zeros(get_dims(slices)) #Initialize a zero image

    (xspace,yspace,zspace,changing_dimension) = get_spacing(slices)

    image_as_list = sort_slices(slices, changing_dimension)

    for z_index, image in enumerate(image_as_list):
        vol_image[:,:,z_index] = image.pixel_array

    return (vol_image, (xspace,yspace,zspace))

def get_sort_coordinate(slice,ddim):
    """Gets the value to sort a slice by, given the changing coordinate"""

    return slice.ImagePositionPatient[ddim]

def sort_slices(slices,ddim):
    """Sorts slices based on the ddim value of ImagePositionPatient, in increasing order"""

    list_of_slices = list(slices)

    return sorted(list_of_slices, key = lambda sls: get_sort_coordinate(sls,ddim))


def get_dims(slices):
    """Given a set of slices, gets number of pixels along x,y,z of reconstructed
    volumetric image."""

    ds = next(iter(slices)) #Takes an arbitrary element of slices
    x_count = ds.Rows
    y_count = ds.Columns
    z_count = len(slices)

    return (x_count,y_count,z_count)

def get_spacing(slices):
    """Given a set of slices, gets spacings between x,y,z for reconstructed
    volumetric image. Also returns the changing dimension in the ImagePositionPatient
    (0 = x, 1 = y, 2 = z)"""


    ## Note: This function works in image coordinates, i.e. X is the spacing between
    # left-right pixels, Y is the spacing between top-bottom pixels, and
    # Z is the spacing between images. To work in RCS (patient) coordinates, see the
    # get_rotation function

    list_of_slices = list(slices)

    # Get two testing slices to determine which coordinate varies
    test_ds1 = list_of_slices[-1]
    test_ds2 = list_of_slices[-2]

    # Test slice difference
    dx = abs(test_ds1.ImagePositionPatient[0] - test_ds2.ImagePositionPatient[0])
    dy = abs(test_ds1.ImagePositionPatient[1] - test_ds2.ImagePositionPatient[1])
    dz = abs(test_ds1.ImagePositionPatient[2] - test_ds2.ImagePositionPatient[2])

    # Set the inter-image delta and the intra-image deltas
    if dx > dy and dx > dz:
        delta_Z = dx
        ddim = 0  # X varies between images
    elif dy > dx and dy > dz:
        delta_Z = dy
        ddim = 1  # Y varies between images
    elif dz > dx and dz > dy:
        delta_Z = dz
        ddim = 2  # Z varies between images
    else:
        raise ValueError("Variation between images is wrong!")

    delta_X = test_ds1.PixelSpacing[0]
    delta_Y = test_ds1.PixelSpacing[1]

    # # Ensure that all the changes between slices are roughly the same (10%)
    # This is slow: do not use unless debugging is needed
    # for i in range(len(list_of_slices) - 1):
    #     ts1 = list_of_slices[i]
    #     ts2 = list_of_slices[i+1]

    #     diffZ = abs(ts1.ImagePositionPatient[ddim] - ts2.ImagePositionPatient[ddim])
    #     diffX = abs(ts1.PixelSpacing[0] - ts2.PixelSpacing[0])
    #     diffY = abs(ts1.PixelSpacing[1] - ts2.PixelSpacing[1])

    #     if abs(diff - delta_Z) > 0.1 * delta_Z:
    #         raise ValueError("DICOM appears to change slice thickness!")

    #     if abs(delta_Y - diffY) > 0.1 * delta_Y or abs(delta_X - diffX) > 0.1 * delta_X:
    #         raise ValueError("DICOM appears to change inter-pixel distances!")

    return (delta_X,delta_Y,delta_Z, ddim)
