#! /bin/bash

#########
# Script to convert a ma file to an off file
#########

inf=$1
outf=$2

num_v=$(grep -c "^v " "$inf")
num_f=$(grep -c "^f " "$inf")

# First will replace file
echo "OFF" > "$outf"
# then append
echo "$num_v $num_f 0" >> "$outf"

# Replace "v " with nothing, also strip off the last number (radius of vertex)
grep "^v " "$inf" | sed -e 's/v //' -e 's/[^ ]\+$//' >> "$outf"
# .ma files are base-0 for the faces. Same as .ma
# Change the "f " at the beginning to the number of vertices of each face.
grep "^f " "$inf" | awk '$1=(NF-1)' >> "$outf"
