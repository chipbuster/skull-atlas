#!/usr/bin/env python3

import dicom
import os
import sys
import re

def main(filepath):

    numeric = re.compile(r'\d*')

    if filepath[-1] == '/':
        print("Warning: This script will not work with a trailing / in the path")

    for dirpath , dirname , files in os.walk(filepath):

        # We are in a directory called DICOMUNC: found the uncompressed DICOMS!
        if os.path.basename(dirpath) == "DICOMOBJ":
            for fname in files:
                target = os.path.join(dirpath,fname)
                ds = dicom.read_file(target)

                if 'ImageOrientationPatient' in ds:
                    o = ds.ImageOrientationPatient
                    for v in o:
                        if abs(v) > 0.1 and abs(abs(v) - 1) > 0.1:
                            pc = dirpath.split('/')
                            patient_id = pc[-2]
                            print("File " + fname + " for Patient " + patient_id + " has wonky orientation data: " +str(o))

if __name__ == "__main__":
    main(sys.argv[1])
