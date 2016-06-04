#!/bin/bash

## A script to streamline the process for meshing volumetric data. It takes
## an existing rawiv and does the following:
##     1. Downsample
##     2. Filter (Bilinear)
##     3. Meshing

# Point this to the build directory of VolRover. It should contain directories
# like bin/, lib/, include/, and have working binaries in bin/
VOL_ROVER=/workspace/software-fresh/VolumeRover-Qt4/Release

ds=3  #Downscaling factor
iso_low=1800 #Low-isovalue for meshing (forms interior of mesh)
iso_high=2500 #Forms exterior of mesh

DOWNSCALED=5female17.d$ds.rawiv        #Name for output of downscaling
FILTERED=5female17.d$ds.filter.rawiv  #Name for output of filtering

if [ ! -f "$DOWNSCALED" ]; then
  $VOL_ROVER/bin/volResample /net/thalamus/workspace2/Cranial-Processed/5female17.rawiv $DOWNSCALED $ds $ds $ds
else
  printf "\t [WARN] Cowardly refusing to overwrite file %s\n" "$DOWNSCALED"
fi

if [ ! -f "$FILTERED" ]; then
  $VOL_ROVER/bin/volfilter 5female17.d$ds.rawiv 0 0 "$FILTERED"
else
  printf "\t [WARN] Cowardly refusing to overwrite file %s\n" "$FILTERED"
fi

$VOL_ROVER/bin/Mesher --isovalue $iso_low --inner_isovalue $iso_high -i "$FILTERED" -o 5female17.d$ds.filter_${iso_low}_${iso_high}.raw --operation mesh
