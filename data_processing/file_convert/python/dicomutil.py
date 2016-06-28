#!/usr/bin/env python3

import numpy as np
import dicom
import sys

def get_image(ds_open_file):
    """Returns a numpy array containing the actual image data of the image,
    in ascending monochrome (high = white), known in DICOM as MONOCHROME2"""

    ds = ds_open_file

    # Photometric Interp. tells us how to convert to MONOCHROME2
    # DICOM standard says this value MUST exist at [0x28,0x4]
    # See http://www.medicalconnections.co.uk/kb/Photometric_Interpretations
    pminterp = ds[0x28,0x4].value
    pixels = ds.pixel_array

    ## MONOCHROME2 is what we want. Do a sanity check and exit
    if pminterp == 'MONOCHROME2':
        if len(pixels.shape) != 2:
            raise ValueError("DICOM claims that this image " +
                             "is grayscale but that's doesn't seem right.")
        return pixels

    ## MONOCHROME1 is almost right, but we need to invert the pixels
    elif pminterp == 'MONOCHROME1':
        if len(pixels.shape) != 2:
            raise ValueError("DICOM claims that this image " +
                             "is grayscale but that's doesn't seem right.")

        ## Need to reverse the values for white-hot (instead of black-hot)
        if "BitsStored" in a:
            maxval = 2**a.BitsStored - 1
        else:
            print("[WARN] Could not find Bits Stored field. Assuming 12-bit pixels")
            maxval = 4095 #A sane default (12-bit scanner)

        # Invert image: set 0 -> max, 1 -> max - 1, etc.
        for x in np.nditer(pixels, op_flags=['readwrite']):
            x[...] = maxval - x # nditer rules

        return pixels

    ## For RGB, we need to convert down to monochrome.
    elif pminterp == 'RGB':
        # We use the default values provided by DCMTK for this: red = 0.299, green = 0.587, blue = 0.114
        # See http://support.dcmtk.org/docs/classDicomImage.html#80077b29cef7b9dbbe21dc19326416ad

        if len(pixels.shape) != 3 or pixels.shape[2] != 3:
            raise ValueError("DICOM claims that this image " +
                             "is RGB but that's doesn't seem right.")

        colorweights = [0.299, 0.587, 0.114]
        output = np.mean(pixels * colorweights, axis=2) #This line may be suspect

        return output

    ## How is this encoded??
    else:
        raise ValueError('I tried to get the image data for this image, '
                         +'but its PhotometricInterpretation value is: ' + pminterp
                         +'which I do not understand.')

def img_coord_to_rcs(ds_open_file):
    """Returns a matrix indicating how to transform image coordinates into RCS (body)
    coordinates."""

    fudge = 1e-4 #If anything is less than this away from 0/1, round

    coords = ds_open_file.ImageOrientationPatient

    only_ones = True

    # Round close coordinates to 0 or \pm 1
    for val in coords:
        if abs(val) < fudge:
            val = 0
        elif abs(val) - 1 < fudge:
            val =  ((val > 0) - (val < 0)) * 1 #+1 or -1 depending on sign

        if val != 0 and val != 1:
            only_ones = False


    if only_ones == False:
        print(coords)
        raise ValueError("Non-right-angle rotations in DICOM image. Aborting...")

    # Construct matrix for transformation
    x_trans = np.array(coords[0:2])
    y_trans = np.array(coords[3:5])
    z_trans = np.cross(x_trans, y_trans)

    # Not technically a permutation matrix since it can have -1 entries
    permut_mat = np.transpose(np.matrix(np.vstack((x_trans,y_trans,z_trans))))

    return permut_mat

def get_value(dicoms,valstring):
    """Given a set of DICOM images, searches for valstring in the DICOM data.
    Returns the first found value among dicoms."""

    for dicom in dicoms:
        if valstring in dicom:
            return dicom.data_element(valstring).value

    # Didn't find it
    print("Could not find tag " + valstring)
    return None

def get_series_metadata(series):
    smetadata = dict()

    data_names = ['SeriesDate', 'SeriesTime', 'SeriesDescription', 'SliceThickness', 'KVP',
                  'PhotometricInterpretation' ]

    for data_name in data_names:
        smetadata[data_name] = get_value(series, data_name)

    # Add dimensions
    smetadata['x_ct'] = get_value(series,'Rows')
    smetadata['y_ct'] = get_value(series,'Columns')
    smetadata['z_ct'] = len(series)

    return smetadata

def get_study_metadata(study):
    smetadata = dict()

    data_names = ['StudyDate', 'StudyTime', 'StudyDescription', 'PatientAge', 'PatientBirthDate',
                  'PatientID', 'PatientSex']

    for data_name in data_names:
        smetadata[data_name] = get_value(series, data_name)

    return smetadata

def get_dicom_file(filename):
    return dicom.read_file(filename)
