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

Once you've decided on a prefix, make a build directory, `cd` into it, and run

`cmake <path-to-source> -DCMAKE_INSTALL_PREFIX=<path-to-prefix>`

Then run `make` and `make install`.

If all has gone well, you'll have a directory with a `bin`/`lib`/`include`/`share` 
structure. All the executables will be inside `bin`.

## Running

Before you begin, make sure you have space: the final product will amount to
over 2GB per patient.

There are two steps to getting data: unpacking the zipfiles and generating 
rawiv from DICOM.

The first step is done with the `build_data_tree` script. It assumes that you
have the Skull Atlas Box directory on your filesystem. If you do not, download
the directory from Box and unzip the resulting zip to get the same effect.

If you are syncing from Box, we don't want to pollute the original directory,
so find a directory you want to do the work in. Then run

`build_data_tree <Box Directory> <work directory>`

(Unless you've changed the name, the Box Directory should almost certainly
be named "Skull Atlas". Make sure you escape the space somehow.)

This will generate a working tree in `<work directory>` with uncompressed
DICOM images and the structure mentioned in the `README.md` in zipextract.

This will also generate a directory called `patient_links`. It is a flat
structure which allows you to easily access patient data without having
to trawl through a tree structure.

(Warnings about unable to read pixmap can be ignore unless there's over 10
of them for a single patient.)

The second step (rawiv generation) is accomplished with the `convert_all` script.
Simply pass it the work directory (the second argument to `build_data_tree`)
and it will generate rawiv and metadata JSONs.

The rawIV files will go to `VOLIMG` and the metadata to `METADATA` for each
patient, as described in the `zipextract` README.
