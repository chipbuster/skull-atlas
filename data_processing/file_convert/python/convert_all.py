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

# Unfortunately, this thing leaks memory like a mother.

import sys
import os
import json
import gc

import dicom

import dicomutil
import rawivutil
import multiprocessing
from slices2rawiv import *

def patient_dicom_to_rawiv(inp_tuple):
    (path,patient_id) = inp_tuple

    dicom_dir = os.path.join(path,'DICOMUNC')
    rawiv_dir = os.path.join(path,'VOLIMG')
    mdata_dir = os.path.join(path,'METADATA')
    all_dicoms = read_all_dicom_in_dirpath(dicom_dir)

    binned_dicoms = list(sep_into_bins(all_dicoms).values())

    counter = 1

    print("Processing DICOM data for patient " + patient_id)

    ## First, dump the study metadata
    study_mdata = dicomutil.get_study_metadata(all_dicoms)
    study_mdata['PatientID'] = patient_id #Override Study settings
    study_mdata_file = os.path.join(mdata_dir, 'study.json')

    with open(study_mdata_file,'w') as outfile:
        json.dump(study_mdata,outfile)

    for series in binned_dicoms:
        if len(series) < 100:
            for s in series:
                del s
            del series
            continue #This DICOM is too short to produce good data

        # Gather interesting metadata on this file
        series_mdata = dicomutil.get_series_metadata(series)
        series_mdata['PatientID'] = patient_id #Overwrite this field -- different from our IDs

        series_name = "series_%d" % counter
        counter += 1

        volfile_name = series_name + ".rawiv"
        mdtfile_name = series_name + ".json"

        vol_out = os.path.join(rawiv_dir, volfile_name)
        mdt_out = os.path.join(mdata_dir, mdtfile_name)

        try:
            (img, (xs,ys,zs)) = make_one_volimage_with_spacings(series)
        except ValueError as verror:
            print("While trying to process series " + series_name + " for patient " + patient_id +
                  "\n" "in directory " + path + " I found a DICOM error which does not allow me to proceed."
                  "\nThe exception message is "+ str(verror) +
                  "\nI am abandoning this series and continuing.")
            continue

        # Write the metadata for the study
        with open(mdt_out,'w') as outfile:
            json.dump(series_mdata, outfile)

        # Write the image
        rawivutil.writeRawIV(img,vol_out,(xs,ys,zs))
        del img

    print("Done processing DICOM data for patient " + patient_id)

def main(path):

    datadirs = []

    for base, dirlist, filelist in os.walk(path):
        if "DICOMUNC" in dirlist and "VOLIMG" in dirlist:
            patient_id = base.split('/')[-1]
            datadirs.append((base,patient_id))

    print("Finished constructing directory listing")

#    ncpu = multiprocessing.cpu_count()
    pool = multiprocessing.Pool(processes=3)

    pool.map(patient_dicom_to_rawiv, datadirs)

if __name__ == '__main__':
    main(sys.argv[1])
