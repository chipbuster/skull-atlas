#! /bin/bash

source config.sh

input=$1
output=$2
r_sigma=${3:200} # radial sigma
s_sigma=${4:1.5} # spacial sigma
f_rad=${5:2}     # filter radius

$VOLROVER_BIN/volfilter $input 0 0 $output $r_sigma $s_sigma $f_rad
