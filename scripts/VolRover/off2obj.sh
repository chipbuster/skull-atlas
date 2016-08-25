#! /bin/bash

#########
# Script to convert a off file to an obj file
#########

inf=$1
outf=$2

off=`head -n 1 "$1"`
header=`head -n 2 "$1" | tail -n 1`

num_v=`echo $header | cut -f 1 -d ' '`
num_f=`echo $header | cut -f 2 -d ' '`
echo "num vertices is $num_v"
echo "num faces is $num_f"

# First will replace file
(
cat <<EOF
# file written from off2obj conversion
# original file is [$inf]
#
# $num_v vertices
# $num_f faces
#
# ------------------------------------------
EOF
) > "$outf"

# then append
# Get the number of vertices
# Ignore the first two lines (OFF line plus NUM_V NUM_F)
vert_line=$((num_v + 2))
head -n $vert_line "$inf" | tail -n $num_v | sed 's/^/v /' >> "$outf"
# Get the number of faces, stripping off the first number
tail -n $num_f "$inf" | sed 's/^[0-9]\+ /f /' | perl -lpe 's/(\d+)/$1+1/ge' >> "$outf"
