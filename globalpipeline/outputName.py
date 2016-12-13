#!/usr/bin/env python3
import sys
import os

import sys

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

## Simple script to calculate correct output name for a file. Script consists
# of three parts:
#
#      1. Split filenames into basename and dirname
#      2. Manipulate basename
#      3. Reconstruct the full path
#
#      Assumptions:
#         Each image file will have a suffix that we can use
#         Each image file will have a single underscore that tells us what
#            stage it's in, that can be changed for the output
#


# Example: outputName img/derp_stage2.inr stage3 ----> img/derp_stage3.inr


filename = sys.argv[1]
stage    = sys.argv[2]   # The stage to name the file after

basename = os.path.basename(filename)
dirname = os.path.dirname(filename)

(nonsuffixedName, suffix) = basename.split('.')

# Check number of underscores: if the count is wrong, reject and crash
underscoreCt = nonsuffixedName.count('_')
if underscoreCt == 0:
    coreName = nonsuffixedName
elif underscoreCt == 1:
    (coreName, _) = nonsuffixedName.split('_')
else:
    print("You can only have one underscore in the name of a file in the pipeline.")
    print("Exiting with error")
    sys.exit(2)

outputname = coreName + "_" + stage + "." + suffix
output = os.path.join(dirname, outputname)

print(output,end="")
