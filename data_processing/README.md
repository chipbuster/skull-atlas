Data Processing Pipeline
========================

NOTE: This does not work on OSX because OSX decided to do a million subtle
little things differently and I'm not going to fix them all right now.

------------------------------------

Implements a data processing pipeline to take trees of Zip files from DCMCCT to
a full dataset, including metadata and rawiv files.

Included is a copy of the [Grassroots DICOM toolkit](https://sourceforge.net/projects/gdcm/), 
which is used to decompress the DICOM images so that [PyDICOM](https://sourceforge.net/projects/gdcm/)
can be used to extract their metadata and form them into rawiv files.

The target directory structure is as follows:

* A top-level directory that is named with the identifier ($NUMID)
   * A directory named `DICOMOBJ` that contains the DICOM data as given to us by DCMCCT
   * A directory named `DICOMUNC` that contains the uncompressed DICOM images (for use by PyDICOM)
   * A directory named `VOLIMG` that contains the volumetric images (e.g. rawiv/inr)
   * A directory named `METADATA` that contains the processed and raw metadata

   TENTATIVE SUGGESTIONS:
   - A directory named PIPELINE that holds temporary files used by the processing pipeline

Prerequisites are:
 
 * A C/C++/Fortran compiler
 * Python 3
 * PyDICOM
 * Numpy
 * CMake
    
## Install Instructions for ICES CentOS 7

To install Numpy, do `easy_install-3.3 --user numpy`. You may need to run this 
twice, since CentOS can be a bit pouty for no good reason.

To install PyDICOM, do `pip-python3 install --user pydicom`

The other prerequisites should already be installed. I've found that the following
module combination works: I haven't tested any others.

`module load c7 gcc/5.2 cmake/3.3.2`

Now you can get ready to install this stuff. Because of the vagaries of CMake, you
need to actually install the project for it to work. Let me emphasize that: **you
cannot just build this project: you must run an install step for things to work!**
This means that you will need a valid value for `CMAKE_INSTALL_PREFIX`.
