#!/usr/bin/env python3

import numpy as np
import dicom
import sys
import os
import dicomutil
from rawivutil import *
from collections import defaultdict

def read_all_dicom_in_dirpath(path):
    """Reads all the dicom images in a path into a set"""

    all_dicom = list()

    for fname in os.listdir(path):
        dicomobj = dicom.read_file(os.path.join(path,fname))
        dicomobj = dicomutil.get_image_metadata(dicomobj) #Drop raw data
        filename = os.path.join(path,fname)
        all_dicom.append((dicomobj,filename))

    return all_dicom

def sep_into_bins(inp):
    """Given a group of (DICOM slice,filename) tuples, separate out into sets
    of DICOM images partitioned by the SeriesInstanceUID"""

    buckets = defaultdict(list)

    # Extract slice info

    for img,fname in inp:
        siuid = img.SeriesInstanceUID
        buckets[siuid].append((img,fname))

    return buckets

def make_one_volimage_with_spacings(slices_w_names):
    """Takes a list of (DataSet, filename) objects (where datasets have no PixelData)
    and renders them into a numpy array. This numpy array is ready to be written
    to file w/ writeRawIV. The function also returns the image spacings.
    The DataSet objects should all come from
    the same DICOM series: this is not enforced, and strange results may happen
    if this requirement is not met."""

    slices = [ x for (x,_) in slices_w_names ]
    vol_image = np.zeros(get_dims(slices)) #Initialize a zero image

    (spacings,changing_dimension) = get_spacing(slices)

    image_as_list = sort_slices(slices_w_names, changing_dimension)

    # Build the image one slice at a time
    for z_index, image in enumerate(image_as_list):
        (_,filename) = image
        ds = dicom.read_file(filename)
        vol_image[:,:,z_index] = dicomutil.get_image(ds)
        del ds

    # Get the Hounsfield coefficients and convert the image to HU

    hounsfield_example_image = next(iter(slices))
    (h_slope, h_intercept) = dicomutil.get_hounsfield_coeffs(hounsfield_example_image)

    vol_image = h_slope * vol_image + h_intercept

    # Define spacings
    xspace = spacings.item(0)
    yspace = spacings.item(1)
    zspace = spacings.item(2)

    return (vol_image, (xspace,yspace,zspace))

def get_sort_val(myslice,ddim):
    """Gets the value to sort a slice by, given the changing coordinate. Can handle either
    tuples of (dicom,filename) or a simple dicom object"""

    if type(myslice) == tuple:
        return myslice[0].ImagePositionPatient[ddim]
    else:
        return myslice.ImagePositionPatient[ddim]



def get_orientation(slices):
    """Gets orientation data from slices."""

    myslice = next(iter(slices))

    return myslice.ImageOrientationPatient

def sort_slices(slices,ddim):
    """Sorts slices based on the ddim value of ImagePositionPatient, in increasing order"""

    list_of_slices = list(slices)

    # Since sls is a tuple, extract the DICOM (0th element of sls) and sort w/ that
    return sorted(list_of_slices, key = lambda sls: get_sort_val(sls,ddim))


def get_dims(slices):
    """Given a set of slices, gets number of pixels along x,y,z of reconstructed
    volumetric image."""

    ds = next(iter(slices)) #Takes an arbitrary element of slices
    x_count = ds.Rows
    y_count = ds.Columns
    z_count = len(slices)

    return (x_count,y_count,z_count)

# this function is a bit...confusing, given its history
def get_spacing(slices):
    """Given a set of slices w/ fnames, gets spacings between x,y,z for reconstructed
    volumetric image. This is returned as a numpy matrix, column format.
    Also returns the changing dimension in the ImagePositionPatient
    (0 = x, 1 = y, 2 = z)"""

    ## Note: This function works in image coordinates, i.e. X is the spacing between
    # left-right pixels, Y is the spacing between top-bottom pixels, and
    # Z is the spacing between images. To work in RCS (patient) coordinates, see the
    # get_rotation function

    list_of_slices = list(slices)

    # Get two testing slices to determine which coordinate varies
    test_ds1 = list_of_slices[-1]
    test_ds2 = list_of_slices[-2]

    # Test difference in coordinate slices
    dx = abs(test_ds1.ImagePositionPatient[0] - test_ds2.ImagePositionPatient[0])
    dy = abs(test_ds1.ImagePositionPatient[1] - test_ds2.ImagePositionPatient[1])
    dz = abs(test_ds1.ImagePositionPatient[2] - test_ds2.ImagePositionPatient[2])

    # Set the inter-image delta and the intra-image deltas
    if dx > dy and dx > dz:
        ddim = 0  # X varies between images
    elif dy > dx and dy > dz:
        ddim = 1  # Y varies between images
    elif dz > dx and dz > dy:
        ddim = 2  # Z varies between images
    else:
        raise ValueError("Variation between images is wrong!")

    # The test slices may not have been adjacent. This is okay for determining
    # the change axis, but may lead to incorrect spacings. Redo the computation
    # with a sorted list and grab the spacing for adjacent slices

    list_of_slices = sort_slices(list_of_slices, ddim)

    test_ds1 = list_of_slices[0]
    test_ds2 = list_of_slices[1]

    delta_Z = abs(get_sort_val(test_ds1,ddim) - get_sort_val(test_ds2,ddim))

    delta_X = test_ds1.PixelSpacing[0]
    delta_Y = test_ds1.PixelSpacing[1]

    # # Check orientation of spacings
    # spacings = np.matrix([delta_X,delta_Y,delta_Z]).T
    # try:
    #     delta_matrix = dicomutil.img_coord_to_rcs(test_ds1)
    # except AttributeError:
    #     delta_matrix = dicomutil.img_coord_to_rcs(test_ds2)

    # new_spacings = delta_matrix * spacings

    new_spacings = np.matrix([delta_X,delta_Y,delta_Z]).T

    return (new_spacings, ddim)
