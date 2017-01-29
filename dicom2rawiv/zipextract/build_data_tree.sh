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

# Which directory is the script stored in?
export SCRIPTDIR="$(dirname "$(realpath "$0")")"

# APPLICATIONS SECTION
###################################################################

# Here we set several variables corresponding to paths programs we need.
case "$OSTYPE" in
    darwin*)  NUMCPUS=$(sysctl -n hw.ncpu) ;;
    linux*)   NUMCPUS=$(grep -c '^processor' /proc/cpuinfo) ;;
    *)        NUMCPUS=1 ;;
esac

# If GNU parallel is installed, use it. Otherwise use xargs.
XARGS=xargs
hash parallel 2>/dev/null && XARGS=parallel
export XARGS

# Grassroots DICOM converter: location may change
CONVERT="$SCRIPTDIR/gdcmconv"
hash $CONVERT 2>/dev/null || echo "The Grassroots DICOM converter was not found at $CONVERT"
export CONVERT

# Unzip and convert is in its own bash script
UNZIP_AND_PROCESS="$SCRIPTDIR/unzip_and_process"

###################################################################
# END APPLICATIONS SECTION

###################
## BEGIN PROGRAM ##
###################

# If either $1 or $2 is empty, give usage info
if [ "${1:-undef}" = "undef"  ] || [ "${2:-undef}" = "undef" ] || [ "$1" = "-help" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0 <Box Sync Directory> <Working Directory>"
    echo ""
    echo "The Box directory should be the path to the Box \"Skull Atlas\""
    echo "directory. Make sure you quote it, else bad things might happen."
    echo "The working directory should be somewhere where you have write"
    echo "permissions. The Box dir will be copied there before being extracted."
    echo ""
    exit 0
fi

# If $3 shows up, the user might have word split a directory by accident:
# "Skull" "Atlas" "/path/to/work".
if [ ! -z "${3+x}" ]; then
    echo "Warning: Argument 3 is not empty. Are your directories quoted correctly?" >> /dev/stderr
    echo "Sleeping 3 seconds to allow user to quit Ctrl+C..." >> /dev/stderr
    sleep 3
fi

IN_DIR_UNPACK=NO
BOXDIR="$(pwd)"
WORKDIR="$(pwd)"

#Not documented--let the user force an in-place unpacking. Untested!
if [ "$1" = "--force" ]; then
    WORKDIR="$(pwd)/$2"
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
    WORKDIR="$(realpath "$2")"
fi

# If not forcing an in-place unpacking, copy Box data to working dir
if [ $IN_DIR_UNPACK = "NO" ]; then
    echo "Copying Box directory to working directory..."
    echo "	($BOXDIR --> $WORKDIR)"
    cp -rp "$BOXDIR" "$WORKDIR"
    echo "Done!"
fi

# Set up a directory to hold softlinks to patients in tree
LINKDIR="${WORKDIR}/patient_links"

#########################
## DIRECTORY DIRECTORY ##
#########################

# On the first pass through the structure (the "unpacking loop"), we record the
# directories with actual data in them. Later, this can be used to speed up the
# search for other loops/programs.

DIRDIR_FILE="/tmp/.datadirdir"
rm -rf "$DIRDIR_FILE" #If script runs twice without cleaning, could place
                      #same directory in here twice. Scrub to avoid that.

# Remainder of script should take place within WORKDIR
cd "$WORKDIR"

##########################
## BEGIN UNPACKING LOOP ##
##########################

# From here, we want to loop over each directory in the directory tree of WORKDIR
# and unzip the archive. From there, we will have to handle several different cases
# to make sure that we handle things correctly.

find $(pwd) -type d | while read DIR; do #Loop over all directories in pwd

    # If we find a numeric-only name, this directory might have already been unpacked
    if [[ "$(basename "$DIR")" != *[!0-9]* ]]; then
        echo "[WARN]: Numeric directory name found!"  >> /dev/stderr
        echo "Have you already partially processed this tree?" >> /dev/stderr
        continue
    # If we find SynapseMediaSets, we need to remove it and skip it so we don't try to walk it by mistake
    elif [ "$(basename "$DIR")" = "SynapseMediaSets" ]; then
        echo "[WARN]: Found SynapseMediaSet in directory. Culling..." >> /dev/stderr
        rm -rf "$DIR"
        continue
    fi

    if [ -d "$DIR" ]; then
        cd "$DIR" #Unzip dumps content in current working dir, so cd to $DIR to let it happen there
    else
        continue #Probably removed by the SynapseMediaSets culler, skip this directory
    fi

    # Check that zipfiles exist in this directory
    for f in "$DIR"/*.zip; do
        [ -e "$f" ] && ZIP_XIST=YES || ZIP_XIST=NO
        break
    done

    # If ZIPS exist, keep processing. Otherwise, skip this directory.
    if [ "$ZIP_XIST" = NO ]; then
        continue
    else
        echo "Extracting images in $DIR"
    fi

    # Loop over all the found .zip files and unzip/process them. This used to be in a loop,
    # has been moved to parallel for faster work. unzip_and_process is a bash function.
    find "$(pwd)" -maxdepth 1 -name '*.zip' -print \
        | "$XARGS" -d '\n' -P $NUMCPUS -I {} $UNZIP_AND_PROCESS {} \""${DIR}"\" \""$DIRDIR_FILE"\"
done

########################
## END UNPACKING LOOP ##
########################

## We can now pass the extracted images to the RAWIV generation. Oh wait, we
## can't, because it's in a compressed form which PyDICOM can't handle. Arrrrgh.
## To solve this, use gdcmconv (bound to $CONVERT variable) to convert each
## image into a RAW encoding. Store these in DICOMUNC as specified by README.

cd "$WORKDIR"

echo ""
echo "######################################################"
echo "# Preparing to convert compressed DICOM to RAW DICOM #"
echo "# The message \"Could not read (pixmap)\" here most    #"
echo "# likely indicates corrupted or missing DICOM data.  #"
echo "######################################################"

mkdir "$LINKDIR"

cat "$DIRDIR_FILE" | while read DIR; do #Loop over all directories in pwd

    cd "$DIR"

    if [ ! -e "$LINKDIR/$(basename "$DIR")" ]; then
        ln -s "$DIR" "$LINKDIR"
    fi

    if [ ! -d "DICOMOBJ" ]; then
        continue # We are not needed here.
    fi

    if [ ! -d "DICOMUNC" ]; then
        echo "[ ERR ] DICOMUNC not found!" >> /dev/stderr
        echo "In path $DIR" >> /dev/stderr
        exit 1
    fi

    NUMID="$(basename "$DIR")"

    echo "Decoding images for patient $NUMID"

    ## Allow pipe failures for this part of the script--some images are inherently corrupted
    set +o pipefail

    ## We should now be in a patient's directory. Use Xargs/parallel to convert images.
    cd "DICOMOBJ"
    find . -maxdepth 1 -type f -printf "%f\n" | $XARGS -d '\n' -P $NUMCPUS -I {} "$CONVERT" -w {} ../DICOMUNC/{} || true

    set -o pipefail

done
