#!/bin/bash

### The data from DCMCCT comes in a few zip formats and files. We use this
### script to unpack it and set it up in a sane file format in the tree.

# In order, this script:
#    - Copies the Box tree to a working copy
#    - Unzips the individual zipfiles
#    - Cleans the data (removes excess software)
#    - If appropriate, moves and relabels the data for consistency
#    - (Stretch goal) Generates RAWIV files

# Data is copied out from the Box directory to avoid polluting the central
# repo w/ Box Sync

# Note: this script is relatively conservative: if it encounters anything unexpected,
# it prefers to error out instead of attempt a repair.

# Strict mode for bash: http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail

# If either $1 or $2 is empty, give usage info
if [ "$1" = ""  ] || [ "$2" = "" ] || [ "$1" = "-help" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0 <Box Sync Directory> <Working Directory>"
    echo ""
    echo "The Box directory should be the path to the Box \"Skull Atlas\""
    echo "directory. Make sure you quote it, else bad things might happen."
    echo "The working directory should be somewhere where you have write"
    echo "permissions. The Box dir will be copied there before being extracted."
    echo ""
fi

# If $3 shows up, the user might have word split a directory by accident:
# "Skull" "Atlas" "/path/to/work".
if [ ! -z "${3+x}" ]; then
    echo "Warning: Argument 3 is not empty. Are your directories quoted correctly?"
    echo "Sleeping 3 seconds to allow user to quit Ctrl+C..."
    sleep 3
fi

IN_DIR_UNPACK=NO
BOXDIR="$(pwd)"
WORKDIR="$(pwd)"

#Not documented--let the user force an in-place unpacking. Untested!
if [ "$1" = "--force" ]; then
    WORKDIR="$2"
    IN_DIR_UNPACK=YES

    echo "######################################"
    echo "WARNING: You are about to FORCE an in-place unpack. This may DESTROY"
    echo "the original data in your directory, and possibly the data in Box!"
    echo "Are you sure you want to do this [Y/n]?"
    echo "######################################"

    read RESPONSE

    if [ "$RESPONSE" = "Y" ]; then
        echo "Forcing..."
    else
        exit -1
    fi

else
    BOXDIR="$1"
    WORKDIR="$2"
fi

# If not forcing an in-place unpacking, copy Box data to working dir
if [ $IN_DIR_UNPACK = "NO" ]; then
    echo "Copying Box directory to working directory..."
    echo "	($BOXDIR --> $WORKDIR)"
    cp -rp "$BOXDIR" "$WORKDIR"
    echo "Done!"
fi

# Remainder of script should take place within WORKDIR
cd $WORKDIR

# From here, we want to loop over each directory in the directory tree of WORKDIR
# and unzip the archive. From there, we will have to handle several different cases
# to make sure that we handle things correctly.


find $(pwd) -type d | while read DIR; do #Loop over all directories in pwd

    if [[ "$(basename "$DIR")" != *[!0-9]* ]]; then #$DIR is valid as a number: probably an existing folder
        echo "[WARN]: Numeric directory name found!"
        echo "Have you already partially processed this directory?"
        continue
    elif [ "$(basename "$DIR")" = "SynapseMediaSets" ]; then
        echo "[WARN]: Found SynapseMediaSet in directory. Culling..."
        rm -rf "$DIR"
        continue
    fi

    if [ -d "$DIR" ]; then
        cd "$DIR" #Unzip dumps content in current working dir, so cd to $DIR to let it happen there
    else
        continue #Probably removed by the basename culler above
    fi


    echo "Now in $(pwd)"

    find "$(pwd)" -maxdepth 1 -name '*.zip' | while read ZIPFILE; do #Loop over all zipfiles in each dir

        NUMID=$(basename -s .zip "$ZIPFILE") #The number assigned to the patient. File is
                                  # guaranteed to have .zip suffix because of our earlier

        unzip -o "$ZIPFILE" > /dev/null

        ## We seek to organize the data into the following structure:
        ##  - A top-level directory that is named with the identifier ($NUMID)
        ##     - A directory named DICOMOBJ that contains the raw DICOM data
        ##     - A directory named VOLIM that contains the volumetric images (e.g. rawiv/inr)
        ##     - A directory named METADATA that contains the processed and raw metadata
        ##
        ##     TENTATIVE SUGGESTIONS:
        ##     - A directory named PIPELINE that holds temporary files used by the processing pipeline

        # Once we unzip the zipfile, there are a few possibilities:

        #  (1) We find a directory with the same NUMID as the zipfile. Great!
        #      Move all of its contents into a tempdir and then rename.

        if [ -d "$NUMID" ]; then
            mkdir ./tempdir
            mv "$NUMID" tempdir/DICOMOBJ
            mv tempdir "$NUMID"
        fi

        # (2) We find a directory named DICOMOBJ. We can fix this by making the $NUMID
        # directory and then moving the DICOMOBJ under it.

        if [ -d "DICOMOBJ" ]; then
            mkdir "$NUMID"
            mv DICOMOBJ "$NUMID"
        fi

        # (3) We find a directory named SynapseMediaSets. This is trickier because
        # the images are in a subdirectory below Syn201[ID]*, where [ID] is some numerical
        # identifier dealing with when the data was accessed. We need to move the DICOMOBJ
        # to up here and clean the remaining files (they are merely clutter to us)

        if [ -d "SynapseMediaSets" ]; then

            # We only expect Syn2016* to be in directory: if more than 1 file, this is unexpected
            if [ $(ls -1 SynapseMediaSets | wc -l ) != "1" ]; then
                echo "Error occured at $DIR: unexpected contents in SynapseMediaSets of $NUMID"
                exit -1
            fi

            # Copy the appropriate DICOMOBJ to top-level if it exists
            if [ -d SynapseMediaSets/Syn*/DICOMOBJ ]; then
                mkdir "$NUMID"
                mv SynapseMediaSets/Syn*/DICOMOBJ "$NUMID"
            else
                #Not good error reporting but I'm on a time budget here :(
                echo "Could not find directory DICOMOBJ in SynapseMediaSets"
                echo "Working in $DIR"
                exit 1
            fi

            # Clean the rest up
            rm -rf SynapseMediaSets
        fi

        # (4) Something else happened and none of the above triggered (currently unexpected)

        if [ ! -d "$NUMID" ]; then
            echo "Something unexpected happened during file processing"
            echo "After unzipping the archive in $DIR"
            echo "I'm seeing $(ls -l)"
            echo ""
            echo ""
            echo "Please report the above error to the script writer.\n"
            echo "###############"
            echo "## Aborting! ##"
            echo "###############"
            exit 1
        fi

        # Now the DICOM images are in $NUMID/DICOMOBJ. Make the other two directories.
        mkdir "$NUMID"/VOLIMG
        mkdir "$NUMID"/METADATA

        ## Now the unpacking is done. We would like to do two things: transform the data
        ## into RAWIV form and place in the PROCIMG directory, and dump the image
        ## metadata into the METADATA directory.

        # For the time being, we will use the MATLAB scripts to accomplish this. This ma
        # be supplanted by a GDCM (Grassroots DICOM) reader program later on.
    done
done
