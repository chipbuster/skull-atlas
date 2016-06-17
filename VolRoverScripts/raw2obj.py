#! /usr/bin/env python3
#############################
# Script that will convert VolRover's .raw file to a more usable (i.e. by Blender)
# .obj file format. It's basically just a regex, except the faces in the .raw file 
# are zero-indexed instead of 1 (reqired by .obj). So we'll do that here.
#############################

import os
import sys

if len(sys.argv) < 2:
  print("usage: raw2obj.py <INPUT.raw>")
  print("  note: will write output to INPUT.obj")
  sys.exit()

infile = sys.argv[1]
outfile = infile[0:-4] + ".obj"
print("infile: ", infile, "outfile:", outfile)

with open(infile, "r") as INF, open(outfile, "w") as OUTF:
  head = INF.readline().split()
  numV = int(head[0])
  numF = int(head[1])

  # Get all the lines in the file
  for i,line in enumerate(INF):
    # The first lines are vertices
    if i < numV:
      OUTF.write("v " + line)
    else:
      # The second lines are faces...
      verts = line.split()
      OUTF.write("f")
      # ...but they're 0-indexed, not 1-indexed. Fix that here.
      for k in range(3):
        OUTF.write(" " + str(int(verts[k])+1))
      # end the line
      OUTF.write("\n")

