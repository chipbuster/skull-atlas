#! /bin/bash

source config.sh

input=$1
output=$2
r_sigma=${3:200} # radial sigma, default 200
s_sigma=${4:1.5} # spatial sigma, default 1.5
f_rad=${5:2}     # filter radius, default 2

$VOLROVER_BIN/volfilter $input 0 0 $output $r_sigma $s_sigma $f_rad
