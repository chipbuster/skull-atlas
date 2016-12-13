#!/usr/bin/env python3

import sys
import os
import numpy as np
import argparse

import inrutil
import rawivutil
import niftiutil

from enum import Enum

# Define valid filetypes in an enumeration
imgFormatStrings = "nii inr rawiv"
IMGType = Enum('IMGType', imgFormatStrings)

def guess_img_format(fname):
    """Guess image format without referencing file suffix."""

    if inrutil.isINR(fname):
        return IMGType.inr
    elif niftiutil.isNii(fname):
        return IMGType.nii
    elif rawivutil.isRawIV(fname):
        return IMGType.rawiv
    else:
        return None

def convert(fnameIn, fnameOut, formatIn, formatOut):
    """Convert the image in the file."""

    # Figure out the input type and read in the appropriate values for data and
    # pixel anisotroy (in the spacings variable)
    if formatIn == IMGType.nii:
        imgObj = niftiutil.readNii(fnameIn)
        pixd = imgObj['pixdim']
        spacings = (pixd[1], pixd[2], pixd[3]) #0 reserved for time dimension
        imgObjdata  = imgObj['data']

    elif formatIn == IMGType.inr:
        imgObj = inrutil.readINR(fnameIn)
        spacings = (imgObj['VX'],imgObj['VY'],imgObj['VZ'])
        imgObjdata  = imgObj['data']

    elif formatIn == IMGType.rawiv:
        imgObj = rawivutil.readRawIV(fnameIn)
        spacings = imgObj['spacings']
        imgObjdata  = imgObj['data']

    else:
        raise ValueError("Invalid value for the input format")

    # Write the appropriate output type
    if formatOut == IMGType.nii:
        niftiutil.writeNii(imgObjdata, fnameOut, spacings)
    elif formatOut == IMGType.inr:
        inrutil.writeINR(imgObjdata, fnameOut, spacings)
    elif formatOut == IMGType.rawiv:
        rawivutil.writeRawIV(imgObjdata, fnameOut, spacings)
    else:
        raise ValueError("Invalid value for the output format")

def main():
    parser = argparse.ArgumentParser(description="Convert between volumetric image formats while\
    intelligently inferring as much formatting information as possible. If names and specified\
    formats do not match, will print a warning.")
    parser.add_argument("inputname", help="Name of input file")
    parser.add_argument('-on','--outputname', help="Name of output file")
    parser.add_argument('-of','--outputformat', help="Output file format",
                        choices=imgFormatStrings.split())
    parser.add_argument('-if', '--inputformat', help=argparse.SUPPRESS,
                        choices=imgFormatStrings.split())
    args = parser.parse_args()

    if not os.path.exists(args.inputname):
        print("The file " + args.inputname + " doesn't appear to exist.")
        sys.exit(-1)

    # Determine the input format
    infileBasename = os.path.basename(args.inputname)
    infileSuffix = infileBasename.split('.')[-1]

    # Attempt to get the filetype from the filename and by reading the file.
    # If these two agree, set inputType to the type. If they disagree, use the
    # format from the filename, but warn. If one is missing, use the other.
    try:
        nameInputType = IMGType[infileSuffix] # See if the suffix is a known type
    except KeyError:
        nameInputType = None

    guessInputType = guess_img_format(args.inputname) # Guess from the file

    if nameInputType == guessInputType:
        inputType = guessInputType
    elif nameInputType is not None and guessInputType is not None\
         and nameInputType != guessInputType:
        print("[WARN]: File suffix and actual image format appear to differ for "
              + args.inputname + ". I'm going with the filename, but you might "
              + "want to double check this one, especially if this script crashes.")
        inputType = nameInputType
    elif nameInputType is None:
        inputType = guessInputType
    elif guessInputType is None:
        inputType = nameInputType
    else:
        raise ValueError("Something cocked up during argument parsing")

    # If we don't have a valid input type, check for an override
    if inputType is None and args.inputformat is None:
        print("[ERROR]: We were unable to determine the input filetype from the\
        filename or data for " + args.inputname +". Please ensure that the data\
        in the file is valid, or pass the input format using the '--inputformat'\
        flag. This flag only works when automated format detection fails--it\
        cannot override a successful format detection.")
        sys.exit(-2)
    elif inputType is None and args.inputformat is not None:
        print("[WARN]: Overriding a bad format detection with " + args.inputformat)
        inputType = args.inputformat

    ### Determine the output type

    # Derived from the filename and an explicit flag on the command line
    fnameOutputType, explicitOutputType = (None, None)

    # No output type given! Fail.
    if args.outputname is None and args.outputformat is None:
        print("[ERROR]: I don't know what output you want! Please provide either "+
              "an output format or an output filename.")
        sys.exit(-3)

    # If a file format is provided, infer the output name from the input file
    if args.outputformat is not None:
        outputPath = os.path.dirname(args.inputname)
        outputFilenameWrong = os.path.basename(args.inputname) #It has the wrong suffix
        outputFilenameCore = outputFilenameWrong.split(".")[:-1] #All but the final suffix

        outputFilename = "".join(outputFilenameCore) + "." + args.outputformat
        explicitOutputType = IMGType[args.outputformat]

    # If a name is provided, infer the output format
    if args.outputname is not None:
        outputFilename = args.outputname
        outputPath = os.path.dirname(args.outputname)
        outputFormatString = os.path.basename(args.outputname).split('.')[-1]

        try:
            fnameOutputType = IMGType[outputFormatString]
        except KeyError:
            print("You gave me a bad suffix--I don't know what output format you want!")

    # Check to make sure these agree
    if fnameOutputType is not None and explicitOutputType is not None\
       and fnameOutputType != explicitOutputType:
        print("[WARN]: Filename and explicitly specified output format disagree." +
              " I will do as you ask and create a file with the wrong suffix. " +
              "Please ensure that this is what you intended to do.")

        outputType = explicitOutputType
        outputFilename = args.outputname
    elif fnameOutputType is None:
        outputType = explicitOutputType
    else:
        outputType = fnameOutputType

    # Join the output path to its filename
    output = os.path.join(outputPath, outputFilename)
    # See if anyone is doing anything sneaky

    if os.path.realpath(args.inputname) == os.path.realpath(output):
        print("[ERROR]: Cowardly refusing to overwrite the input file.")
        sys.exit(-4)

    # We're done! Just call the logic
    try:
        convert(args.inputname, output, inputType, outputType)
    except:
        print("[ERROR]: Conversion program errored. This could be because a bug slipped" +
              " past the argparser, or it could be because you specified a bad input type." +
              " I'm printing the error below for your reference.")
        print("\n")
        raise


if __name__ == "__main__":
    main()

def testSpacing(fname):
    """Dummy function for testing"""
    if formatIn == IMGType.nii:
        img = niftiutil.readNii(fnameIn)
        pixd = img['pixdim']
        spacings = (pixd[1], pixd[2], pixd[3]) #0 reserved for time dimension
    elif formatIn == IMGType.inr:
        imgObj = inrutil.readINR(fnameIn)
        spacings = (imgObj['VX'],imgObj['VY'],imgObj['VZ'])
        imgObjdata  = imgObj['data']
    elif formatIn == IMGType.rawiv:
        imgObj = rawivutil.readRawIV(fnameIn)
        spacings = imgObj['spacings']

    print(spacings)
