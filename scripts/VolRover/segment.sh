#! /bin/bash

source config.sh

# Uses VolRover's
input=$1
output_dir=$2
num_segs=$3

$VOLROVER_BIN/decompose -n $num_segs -P -H -d $output_dir $input 
