#! /usr/bin/env python3

import numpy as np
import sys
import os
from rawivutil import *

def threshold(args):
    '''Thresholds a rawiv file between the two given values. Writes output to
    given filename if output file is provided, otherwise outputs to <infile>_filtered'''

    #If the file is called without arguments:
    if len(args)<4:
        print('Usage: %s inputfile low_threshold hi_threshold [outputfile]')
        print('See source file documentations (docstrings) for more details.')
    else:

        #first extract header information and make a dictionary of header and data info
        dataDict=readRawIV(sys.argv[1])

        #extract spacings, i.e. the span of x,y,z
        spacings=(dataDict['spanX'],dataDict['spanY'],dataDict['spanZ'])

        #this step sets any value lower or higher than the two thresholds to 0
        newDataDict=replaceWithZeros(dataDict, sys.argv[2],sys.argv[3])

        #this step names and writes the output file
        if len(sys.argv)!=5:
            outputName=(str(sys.argv[1])[:-6] + '_filtered' + str(sys.argv[1][-6:]))
            writeRawIV(newDataDict['data'], outputName, spacings)
        else:
            writeRawIV(newDataDict['data'], sys.argv[4], spacings)

if __name__ == '__main__':
    threshold(sys.argv)
