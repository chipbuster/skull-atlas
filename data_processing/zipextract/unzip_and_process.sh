#!/bin/bash

set -euo pipefail

# If using XARGS, we need to strip the quotes off of the names. If not using XARGS,
# this is (probably) still safe


ZIPFILE="$(echo "$1" | tr -d '"')"
DIR="$(echo "$2" | tr -d '"')"
DIRDIR_FILE="$(echo "$3" | tr -d '"')"

NUMID=$(basename -s .zip "$ZIPFILE") #The number assigned to the patient.

TMPDIR=$DIR/${NUMID}_TMP
mkdir "$TMPDIR" # Create temporary working directory
cd "$TMPDIR"

FOUND_FILES="false"

unzip -o "$DIR/$ZIPFILE" > /dev/null #-o forces overwrite if files already exist

function looks_like_dicom(){
    NUM_FILES=$(find "$1" -maxdepth 1 -name '[0-9]*' | wc -l)
    if [ "$NUM_FILES" -ge 30 ]; then
        printf "yes"
    else
        printf "no"
    fi
}

## We seek to organize the data into the structure described in the README.
# Once we unzip the zipfile, there are a few possibilities:

#  (1) We find a directory with the same NUMID as the zipfile. Great!

if [ -d "$NUMID" ]; then
    # (1a) The directory looks like a DICOM directory. Great!
    if [ $(looks_like_dicom "$NUMID") = "yes" ]; then
        mkdir ./tempdir
        mv "$NUMID" tempdir/DICOMOBJ
        mv tempdir "$NUMID"
        FOUND_FILES="true"
    fi
    # (1b) The directory isn't a DICOM, but it has a DICOMOBJ below it
    if [ -d "$NUMID/DICOMOBJ" ]; then
        mkdir ./tempdir
        mv "$NUMID/DICOMOBJ" tempdir
        rm -rf "$NUMID" #Remove the extra cruft
        mv tempdir "$NUMID"
    fi
fi

# (2) We find a directory named DICOMOBJ. We can fix this by making the $NUMID
# directory and then moving the DICOMOBJ under it.

if [ -d "DICOMOBJ" ]; then
    mkdir "$NUMID"
    mv DICOMOBJ "$NUMID"

    FOUND_FILES="true"
fi

# (3) We find a directory named SynapseMediaSets. This is trickier because
# the images are in a subdirectory below Syn201[ID]*, where [ID] is some numerical
# identifier dealing with when the data was accessed. We need to move the DICOMOBJ
# to up here and clean the remaining files (they are merely clutter to us)

if [ -d "SynapseMediaSets" ]; then

    # We only expect Syn2016* to be in directory: if more than 1 file, this is unexpected
    if [ $(ls -1 SynapseMediaSets | wc -l ) != "1" ]; then
        echo "[WARN] at $DIR: unexpected contents in SynapseMediaSets of $NUMID"
    fi

    # Copy the appropriate DICOMOBJ to top-level if it exists
    if [ -d SynapseMediaSets/Syn*/DICOMOBJ ]; then
        mkdir "$NUMID"
        mv SynapseMediaSets/Syn*/DICOMOBJ "$NUMID"
        FOUND_FILES="true"
    else
        #Not good error reporting but I'm on a time budget here :(
        echo "Could not find directory DICOMOBJ in SynapseMediaSets"
        echo "Working in $DIR"
        exit 1
    fi

    # Clean the rest up
    rm -rf SynapseMediaSets
fi

# (4) We find a directory named Syn201[ID]*. This is the subdirectory of the above case
# which has been "helpfully" pre-extracted for us. Basically we need to do the above,
# but without using the wrapping "SynapseMediaSets" directory

if [ "$FOUND_FILES" = "false" ]; then
    true || echo * | grep -E 'Syn[0-9]+' &> /dev/null #Use this exit code to search

    if [ $? -eq 0 ]; then #Found a folder named Syn*
        if [ $(ls -1 | wc -l ) != "1" ]; then
           echo "[WARN] at $DIR: unexpected contents in Syn[0-9]+ of $NUMID"
        fi

        if [ -d Syn*/DICOMOBJ ]; then
            mkdir "$NUMID"
            mv Syn*/DICOMOBJ "$NUMID"
            FOUND_FILES="true"

            # Clean up the remainder
            rm -rf Syn*
        fi
    fi
fi
 
# (FINAL) Something else happened and none of the above triggered (currently unexpected)

if [ ! -d "$NUMID" ]; then
    echo "Something unexpected happened during file processing"
    echo "After unzipping the archive for patient $NUMID"
    echo "in $TMPDIR"
    echo ""
    echo "The files I'm seeing are:"
    echo "$(ls -1)"
    echo ""
    echo ""
    echo "Please report the above error to the script writer."
    echo "###############"
    echo "## Aborting! ##"
    echo "###############"
    exit 1
fi

# If the extraction was successful, do other required things
if [ -d "$NUMID/DICOMOBJ" ]; then
  mkdir "$NUMID"/VOLIMG         # For volumetric images
  mkdir "$NUMID"/METADATA       # For metadata dumps
  mkdir "$NUMID"/DICOMUNC       # For uncompressed DICOM images
  echo "$DIR/$NUMID" >> "$DIRDIR_FILE" # Add this folder to the data folder list

  # Move the directories to the right location and clean the temp
  mv "$NUMID" "$DIR/$NUMID"   # Move created NUMID to top-level
  cd "$DIR"                   # Change dir to target location
  rmdir "$TMPDIR"             # Remove temporary directory
fi

