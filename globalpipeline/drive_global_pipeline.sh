#!/usr/bin/env bash

set -euo pipefail #BASH STRICT MODE ON

## This script is an example of how to drive the current overall
## pipeline. It will undoubtedly be changed or updated as time goes on

## The default name for a file is seriesN.<suffix>. We use the convention
## of naming a file with an underscore and its stage in the pipeline. For
## example, series1.rawiv is the input file, series1_precurate is the
## file after precuration, series1_threshold is the file after thresholding, etc.

####################
## CONFIGURATIONS ##
####################

## You need to modify the variables in this section to fit your system/install

# Path to script-this does not work on MacOS and needs to be manually specified
export SCRIPTDIR="$(dirname "$(readlink -f "$0")")"

# The binary directory of the skull atlas output directory. Needs to be exported
# so that helper functions can see it.
export BINDIR="/home/chipbuster/tmp/skatbld/bin"
#export BINDIR="/workspace/skull-atlas/code/install/bin"

# The directory of the source Box tree
BOXDIR="/home/chipbuster//NASpinny/UTBox/Skull Atlas"
#BOXDIR="/workspace/skull-atlas/data/boxData"

# The working directory where the files will be unpacked to and worked on
WORKDIR="/mnt/spinny/CVC/data-skulls"
#WORKDIR="/workspace/skull-atlas/data/workingData"

# The number of CPUs to use. Both CPU count and memory can become an issue here
# I recommend no more than 4 CPU cores per 16 GB RAM free (not tied up by
# other programs) up to your number of logical CPUs
NCPU=8

# Preserve all images in the pipeline for later examination. If this is any
# other string than "true", the pipeline will periodically cull intermediate
# images from the disk to save space
# PRESERVE_INTERMEDIATES="true"
PRESERVE_INTERMEDIATES="true"

####################################
## HELPER FUNCTIONS AND VARIABLES ##
####################################

# If GNU parallel is installed, use it. Otherwise use xargs.
XARGS=xargs
hash parallel 2>/dev/null && XARGS=parallel
export XARGS

# Utility fn for logging the time that something happened
timestamp_msg(){
    echo ""
    echo "==== TIMESTAMP $(date "+%H:%M:%S") ===="
    echo "$1"
    echo "======================================="
}

#####################
## PIPELINE DRIVER ##
#####################

###
###  Take raw DICOM data and put it into usable form
###

echo ""
echo "Beginning pipeline run"
echo "I will print out when each step is done. Please give each step ~20 minutes"
echo "to complete before killing it as a stuck job."

# Extract zip files from raw data directory to working directory
timestamp_msg "Extracting DICOM data from zip files."
"$BINDIR/build_data_tree" "$BOXDIR" "$WORKDIR"

# Take DICOM Images and extract volumetric images from them.
timestamp_msg "Converting DICOM to volumetric image."
"$BINDIR/dicom_to_rawiv" "$WORKDIR"

# Take each RawIV image and convert it to INR for the precurate binary
timestamp_msg "Converting rawiv to inr in preparation for precuration"
find "$WORKDIR" -name '*.rawiv' -print0 | "$XARGS" -I % -n 1 -0 -P $NCPU "$BINDIR/volimg_convert" % -of inr

# Precurate the images by using Nathan's poorly commented C++ code
timestamp_msg "Precurating images: select largest connected component above certain isovalue"
find "$WORKDIR" -name '*.inr' -print0 | "$XARGS" -I % -n 1 -0 -P $NCPU "$SCRIPTDIR"/pipeline_precurate %

# Convert the images into rawiv to prepare for thresholding and raster expansion
timestamp_msg "Converting precurated image to RawIV for thresholding"
find "$WORKDIR" -name '*_precurate.inr' -print0 | "$XARGS" -I % -n 1 -0 -P $NCPU "$BINDIR/volimg_convert" % -of rawiv

# Threshold the skull voxels by using Kevin's shitty C++ code
timestamp_msg "Thresholding skull voxels"
find "$WORKDIR" -name '*_precurate.rawiv' -print0 | "$XARGS" -I % -n 1 -0 -P $NCPU "$SCRIPTDIR"/pipeline_thresholdGrow %

# Use conversion script to turn precurated skulls into Nifti images
timestamp_msg "Converting thresholded rawivs to Nifti"
find "$WORKDIR" -name '*_filtered.inr' -print0 | "$XARGS" -I % -n 1 -0 -P $NCPU "$BINDIR/volimg_convert" % -of nii

# We have a *lot* of garbage in the directory now. Find the unneeded stuff and kill it off
if [ "$PRESERVE_INTERMEDIATES" != "true" ]; then
    find "$WORKDIR" -name '*.inr' -delete
    find "$WORKDIR" -name '*_precurate.rawiv' -delete
fi


