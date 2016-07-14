#! /Users/iansp/anaconda/bin/python

import numpy as np
import sys
import os
from rawivutil import *

def main():
    '''With this file you can read a rawiv and filter out voxels with values lower than the 1st argument and greater than the second argument. the third argument is optional output file name'''
    
    #If the file is called without arguments:
    if len(sys.argv)<4:
        print ("With this file you can read a rawiv and filter out voxels with values lower than the 1st argument and greater than the second argument")
        print()
        print('In other words, the first argument is the filename, the second is the low threshold, the third is the high threshold, and the fourth is the output filename. if the output filename is not given, a new file will be made with _rastered appended to the input filename')
        
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
main()

