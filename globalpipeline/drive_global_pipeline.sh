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

# The binary directory of the skull atlas output directory. Needs to be exported
# so that helper functions can see it.
#export BINDIR="/home/chipbuster/tmp/skatbld/bin"
export BINDIR="/workspace/skull-atlas/code/install/bin"

# The directory of the source Box tree
#BOXDIR="/home/chipbuster//NASpinny/UTBox/Skull Atlas"
BOXDIR="/workspace/skull-atlas/data/boxData"

# The working directory where the files will be unpacked to and worked on
#WORKDIR="/mnt/spinny/CVC/data-skulls"
WORKDIR="/workspace/skull-atlas/data/workingData"

# The number of CPUs to use. Both CPU count and memory can become an issue here
# I recommend no more than 4 CPU cores per 16 GB RAM free (not tied up by
# other programs) up to your number of logical CPUs
NCPU=16

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

# Xargs commands must be written as one-liners. If this is not possible,
# we can create a function that does what we want and then export it
precurate_one(){
    IMAGE_IN="$1"
    IMAGE_OUT="$("$BINDIR/outputName" "$IMAGE_IN" "precurate")"

    "$BINDIR/precurate" "$IMAGE_IN" "$IMAGE_OUT" 200
}
export -f precurate_one

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
find "$WORKDIR" -name '*.inr' -print0 | "$XARGS" -I % -n 1 -0 -P $NCPU precurate_one %
