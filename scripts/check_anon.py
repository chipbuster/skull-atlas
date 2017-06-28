#!/usr/bin/env python3

import sys
import os
import dicom
import re

targetDir = sys.argv[1]

unknownTarget = re.compile('Unknown')

for dirn, _, filen in os.walk(targetDir):
    try:
        name = os.path.join(dirn, filen)
        ds = dicom.read_file(name)
        patName = ds[0x10,0x10].value #Patient's name
        if not unknownTarget.search(patName):
            print("File " + name + " does not appear to be anon!"
    
    except InvalidDicomError:
        # Not a DICOM. Ignore
        print(os.path.join(dirn,filen) + " is not a DICOM")
        continue

