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

# Unfortunately, this thing leaks memory like a mother. An 8-core run can survive on
# about 20GB of memory--I would not suggest going over that without at least 32 to spare

import sys
import os
import json
import gc

import dicom

import dicomutil
import rawivutil
import inrutil
import niftiutil
import multiprocessing
from slices2rawiv import *

def patient_dicom_to_imgs(inp_tuple):
    # Input comes as a tuple for easier compat with multiprocessing
    # Need to unpack this tuple before proceeding
    (path,patient_id) = inp_tuple

    # Set up directories for later
    dicom_dir = os.path.join(path,'DICOMUNC')
    rawiv_dir = os.path.join(path,'VOLIMG')
    mdata_dir = os.path.join(path,'METADATA')

    dicoms_w_fnames = read_all_dicom_in_dirpath(dicom_dir)
    binned_dicoms = list(sep_into_bins(dicoms_w_fnames).values())

    counter = 1

    print("Processing DICOM data for patient " + patient_id)

    ## First, dump the study metadata
    all_dicoms = [ x for (x,_) in dicoms_w_fnames ]
    study_mdata = dicomutil.get_study_metadata(all_dicoms)
    study_mdata['PatientID'] = patient_id #Override Study settings
    study_mdata_file = os.path.join(mdata_dir, 'study.json')

    with open(study_mdata_file,'w') as outfile:
        json.dump(study_mdata,outfile)

    for series_w_fnames in binned_dicoms:
        # If the series is small, ignore it--probably close-ups or too coarse
        if len(series_w_fnames) < 100:
            continue

        # Give this series an identifier
        series_name = "series_%d" % counter
        counter += 1

        # Extract the actual dicom out of the tuples
        series = [ x for (x,_) in series_w_fnames ]

        # Gather interesting metadata on this series
        series_mdata = dicomutil.get_series_metadata(series)
        series_mdata['PatientID'] = patient_id #Overwrite this field -- different from our IDs
        series_mdata['CVCSeriesId'] = series_name

        rawivfile_name = series_name + ".rawiv"
        inrfile_name = series_name + ".inr"
        niifile_name = series_name + ".nii"
        mdtfile_name = series_name + ".json"

        rawiv_out = os.path.join(rawiv_dir, rawivfile_name)
        inr_out = os.path.join(rawiv_dir, inrfile_name)
        nii_out = os.path.join(rawiv_dir, niifile_name)
        mdt_out = os.path.join(mdata_dir, mdtfile_name)

        # Determine if series is aligned axially. If not, discard it.
        (_,ch_dim) = get_spacing(series)

        # Image needs to change along Z axis
        if ch_dim != 2:
            continue

        # Make a numpy array out of the slices
        try:
            (img, (xs,ys,zs)) = make_one_volimage_with_spacings(series_w_fnames)
        except ValueError as verror:
            print("While trying to process series " + series_name + " for patient " + patient_id +
                  "\n" "in directory " + path + " I found a DICOM error which does not allow me to proceed."
                  "\nThe exception message is "+ str(verror) +
                  "\nI am abandoning this series and continuing.")
            continue

        # Write the metadata for the study
        with open(mdt_out,'w') as outfile:
            json.dump(series_mdata, outfile)

        # Write the image to multiple formats
        rawivutil.writeRawIV(img,rawiv_out,(xs,ys,zs))
        inrutil.writeINR(img,inr_out,(xs,ys,zs))
        niftiutil.writeNii(img,nii_out,(xs,ys,zs))
        del img

    print("Done processing DICOM data for patient " + patient_id)

    if len(os.listdir()) == 0:
        print("[WARN]: Patient " + patient_id + " does not have any z-oriented series!")

def main(path):

    datadirs = []

    ## Generate the argument list
    for base, dirlist, filelist in os.walk(path):
        if "DICOMUNC" in dirlist and "VOLIMG" in dirlist:
            patient_id = base.split('/')[-1]
            datadirs.append((base,patient_id))

    print("Finished constructing directory listing")

    ncpu = multiprocessing.cpu_count()
    pool = multiprocessing.Pool(processes=8)

    ## Serial:
    # for arg in datadirs:
    #    patient_dicom_to_rawiv(arg)

    ## Parallel
    pool.map(patient_dicom_to_imgs, datadirs, chunksize=1)

if __name__ == '__main__':
    main(sys.argv[1])
