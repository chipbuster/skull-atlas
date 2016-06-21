#! /bin/bash

source config.sh

# Will generate mesh from some input, save it to some output, and use the given
# iso values.
input=$1
output=$2
iso_min=$3
iso_max=$4

$VOLROVER_BIN/Mesher --inner_isovalue=$iso_min --isovalue=$iso_max -i $input -o $output --operation mesh
