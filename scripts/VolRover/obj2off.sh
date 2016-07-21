#! /bin/bash

#########
# Script to convert a obj file to an off file
#########

inf=$1
outf=$2

num_v=$(grep -c "^v " $inf)
num_f=$(grep -c "^f " $inf)

# First will replace file
echo "OFF" > $outf
# then append
echo "$num_v $num_f 0" >> $outf

# Replace "v " with nothing
grep "^v " $inf | sed 's/v //' >> $outf
# off files are base-0 for the faces. Need to fix that here.
# Also change the "f " at the beginning to the number of vertices of each face.
grep "^f " $inf | perl -lpe 's/(\d+)/$1-1/ge' | awk '$1=(NF-1)' >> $outf
