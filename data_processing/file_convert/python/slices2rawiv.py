#!/usr/bin/env python3

import numpy as np
import dicom
import sys
import os
from rawivutil import *
from dicomutil import *

def make_one_rawiv(slices):
    """Takes a set of PyDICOM DataSet objects (returned by dicom.read_file) and
    renders them into a rawIV file. The DataSet objects should all come from
    the same DICOM series: this is not enforced, and strange results may happen
    if this requirement is not met."""

    
