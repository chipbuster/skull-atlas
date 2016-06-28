#!/usr/bin/env python3

## This file is the main driver for conversion from DICOM to rawiv. It assumes
# that each patient's data is in a directory named by the patient ID, with
# the following subdirectory structure:
#
#  -[Patient ID]
#     - DICOMOBJ      (for raw DICOM files from DCMCCT)
#     - DICOMUNC      (uncompressed DICOM)
#     - METADATA      (for image metadata dumps)
#     - VOLIMG        (for volumetric image output)
#
# These structures can be generated by running build_data_tree.sh
# It will take a path to the top-level directory of the tree, then traverse the
# tree until it finds a directory that meets the above criteria. It will then
# take all of the images in DICOMUNC and attempt to make rawivs which will output
# into VOLIMG.
#
# The rawiv creation can be stymied for several reasons (missing data,
# missing headers, inconsistent headers, etc.). In these cases, this script will
# attempt to warn the user about the error, but it will not intentionally form
# a rawIV that it knows to be damaged.

import sys
import os
import dicom
import dicomutil
import rawivutil
import multiprocessing
from slices2rawiv import *

def patient_dicom_to_rawiv(path, patient_id):
    all_dicoms = read_all_dicom_in_dirpath(path)

    binned_dicoms = list(sep_into_bins(all_dicoms))

    counter = 1

    for series in binned_dicoms:
        if len(series) < 100:
            continue #This DICOM is too short to produce good data

        # Gather interesting metadata on this file
        series_mdata = get_series_metadata(series)

        # TODO



def main(path):

    datadirs = []

    for base, dirlist, filelist in os.walk(path):
        if "DICOMUNC" in dirs and "VOLIMG" in dirs:
            datadirs.append(base)




if __name__ == '__main__':
    main(sys.argv[1])
