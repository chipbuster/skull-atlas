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
                    continue
                else:
                    # Find patient name and report
                    pc = dirpath.split('/')
                    patient_id = pc[-2]

                    print("File " + fname + " for Patient " + patient_id + " does not have any position info.")

if __name__ == "__main__":
    main(sys.argv[1])
