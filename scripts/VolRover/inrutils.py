#!/usr/bin/env python3

import numpy as np
import struct
import sys

def handleTag(tag_name, tag_val, inr_obj):
    """Handles a tag in the header of the INR file. I'm not sure if these
    are case-sensitive, so just treat them as case-insensitive."""
    if tag_name.upper() == "VDIM":
        assert tag_val == "1", "Error: Cannot work with VDIM not equal to 1 (????)" + tag_val
    if tag_name.upper() in {"XDIM", "YDIM", "ZDIM"}:
        inr_obj[tag_name.upper()] = int(tag_val)
    elif tag_name.upper() in {"VX", "VY", "VZ"}:
        inr_obj[tag_name.upper()] = float(tag_val)
    elif tag_name.upper() == "TYPE":
        # It needs to be one of these.
        assert tag_val.lower() in {"float", "signed fixed", "unsigned fixed"}, \
            "Error!! Unrecognized value type: " + tag_val
 
        inr_obj["TYPE"] = tag_val.lower()
    elif tag_name.upper() == "PIXSIZE":
        px = int(tag_val.split()[0])
        assert px in {8, 16, 32, 64}, \
            "Error!! Unrecognized pixel size: [" + str(px) + "] from str(" + tag_val + ")"

        inr_obj["PIXSIZE"] = px
    elif tag_name.upper() == "SCALE":
        # Ignoring...
        print("Warning: Scale not implemented. Ignoring.")
    elif tag_name.upper() == "CPU":
        # Only(?) allowed values.
        if tag_val in {"decm", "alpha", "pc"}:
            inr_obj["ENDIAN"] = "little"
        elif tag_val in {"sun", "sgi"}:
            inr_obj["ENDIAN"] = "big"
        else:
            assert True, "Error!! Unrecognized endianness: " + tag_val
       
def getCh(infile):
    return chr(infile.read(1)[0])
    #return "" + struct.unpack('s', infile.read(1))[0]

def readINRHeader(infile, inr_obj):
    # Get the first bit
    ch = getCh(infile)

    while ch != '{':
        ch = getCh(infile)

    tag_name = ""
    tag_value = ""
    temp = ""
    while infile.tell() != 256:
        #print("here: ", infile.tell())
        ch = getCh(infile)
        if ch == "=":
            tag_name = temp
            temp = ""
        elif ch == "\n":
            tag_value = temp
            # Also, need to handle the tag
            handleTag(tag_name, tag_value, inr_obj)
            temp = ""
            tag_name = ""
            tag_value = ""
        elif ch == " ":
            # A space should be ignored if temp is empty
            if temp != "":
                temp += ch
        else:
            temp += ch

def readData(infile, io):
    """Actually reads the data from the file. Will determine the pixel type
    and size from io (inria object). Does endian-checking, resizes the array,
    and sets io['data'] to be the correct numpy array. Assumes infile is at
    the correct position (pointing to the first index of data)"""

    # Get the endianness
    end = ""
    if io['ENDIAN'] == "little":
        end = "<"
    else:
        end = ">"

    # Get the data type
    px = getDataType(io['TYPE'], io['PIXSIZE'])

    # Compute and save how many values.
    total_vals = io['XDIM'] * io['YDIM'] * io['ZDIM']
    io['data_size'] = total_vals
    
    # Actually read them from the file.
    arrpacker = struct.Struct((end + "%d" + px) % total_vals)
    inp_bytes = infile.read()
    arr = np.array(arrpacker.unpack(inp_bytes))
    # According to this page: http://stackoverflow.com/questions/34906509/troubles-understanding-inr-file-format
    # inr files are written in column-major order. Reshape it to have this order and correct dims.
    imgdata = np.reshape(arr, (int(io['XDIM']), int(io['YDIM']), int(io['ZDIM'])), order='F')

    # Save in the corresponding location.
    io['data'] = imgdata

def getDataType(p_type, p_size):
    """Given the input type and size, returns the corresponding
    Python character for that data type"""
    # How many bits in a single value?
    if p_type == 'float':
        if p_size == 32:
            return 'f'
        elif p_size == 64:
            return 'd'
        else:   # Anything else is an error.
            assert True, "Error: Cannot have floats and pixel size of " + str(p_size)
    elif p_type == 'signed fixed':
        if p_size == 8:
            return 'b'
        elif p_size == 16:
            return 'h'
        elif p_size == 32:
            return 'i'
        else:   # PIXSIZE of 64
            return 'q'
    else: # TYPE of unsigend
        if p_size == 8:
            return 'B'
        elif p_size == 16:
            return 'H'
        elif p_size == 32:
            return 'I'
        else:   # PIXSIZE of 64
            return 'Q'

def readINR(fname):
    """Returns a dictionary with various entries representing an INR file.
    An INR file consists of the following:
#INRIMAGE-4#{ 
XDIM=128  // x dimension 
YDIM=128  // y dimension 
ZDIM=128  // z dimension 
VDIM=1    // number of scalar per voxel (1 = scalar image, 3 = 3D image of vectors) 
        // Note: VDIM must be 1 for this to work.
VX=0.66   // voxel size in x 
VY=0.66   // voxel size in y 
VZ=1      // voxel size in z 
TYPE=unsigned fixed   // float, signed fixed, or unsigned fixed 
PIXSIZE=16 bits  // 8, 16, 32, or 64 
SCALE=2**0       // not used in my program 
CPU=decm         // decm, alpha, pc, sun, sgi 
                 // little endianness : decm, alpha, pc; big endianness :sun, sgi

// fill with carriage return or with any other information
##} // until the total size of the header is 256 characters (including final newline character) 
// raw data, size=XDIM*YDIM*ZDIM*PIXSIZE/8 

    After this function has been called, the returned object will be a
    dictionary with the following elements defined:
        XDIM,YDIM,ZDIM : int
        VX,VY,VZ : float
        TYPE    : {'float', 'signed fixed', or 'unsigned fixed'}
        PIXSIZE : {8, 16, 32, 64}
        ENDIAN  : {'little', 'big'}
        data_size : number of data elements
        data      : np array of values
    """

    inr_obj = {}
    with open(fname, 'rb') as infile:
        # Get the header.
        readINRHeader(infile, inr_obj)
        
        # If all goes well, these values should be set:
        # read a total of XDIM*YDIM*ZDIM things from raw bits.
        readData(infile, inr_obj)

        return inr_obj
