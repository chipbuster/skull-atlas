#!/bin/bash

VOL_ROVER=/workspace/software-fresh/VolumeRover-Qt4/Release

ds=3
iso_low=1800
iso_high=2500

if [ ! -f 5female17.d$ds.rawiv ]; then
  $VOL_ROVER/bin/volResample /net/thalamus/workspace2/Cranial-Processed/5female17.rawiv 5female17.d$ds.rawiv $ds $ds $ds
fi
if [ ! -f 5female17.d$ds.filter.rawiv ]; then
  $VOL_ROVER/bin/volfilter 5female17.d$ds.rawiv 0 0 5female17.d$ds.filter.rawiv
fi
$VOL_ROVER/bin/Mesher --isovalue $iso_low --inner_isovalue $iso_high -i 5female17.d$ds.filter.rawiv -o 5female17.d$ds.filter_${iso_low}_${iso_high}.raw --operation mesh
