#!/usr/bin/env python3

import numpy as np
import dicom
import sys

def get_dicom_image(ds_open_file):
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

def get_dicom_metadata(ds_open_file):
    """Retrieves DICOM file metadata, dumping the image data and overlays."""

    ds = ds_open_file
    del ds[0x7fe0,0x10] # The pixel data

    # Find all the keys that lead with 0x6000, using the fact that tags are derived from long
    overlay_tags = [ x for x in ds.keys() if x > 0x60000000 and x < 0x60ffffff ]
    for tag in overlay_tags:
        del ds[tag]   #Nuk'd

    return ds

def get_dicom_file(filename):
    return dicom.read_file(filename)
