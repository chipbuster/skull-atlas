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

## Quick Start
 0. Install PyDICOM and Numpy using your mechanism of choice.
 1. Download the Skull Atlas directory from Box. Unzip it (if using the web interface) or make sure it's synced to your computer (if using Box Sync).
 2. Make a directory where you'd like to install things. Run `cmake <path_to_source> -DCMAKE_INSTALL_PREFIX=</path/to/installdir>`. Make sure that `path_to_source` is the top level of this git repository (should be named skull-atlas if you didn't change it).
 3. Run `make`, then `make install`
 4. Run `</path/to/installdir>/bin/build_data_tree <Box Dir> <Data Dir>` where `<Data Dir>` is where you want the data to install.
 5. Run `</path/to/installdir>/bin/convert_all <Data Dir>`.
    
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

Note that the `path-to-source` must be the top-level directory of the skull-atlas code.

Then run `make` and `make install`.

If all has gone well, you'll have a directory with a `bin`/`lib`/`include`/`share` 
structure. All the executables will be inside `bin`.

## Running

Before you begin, make sure you have space: the final product will amount to
over 2GB per patient.

There are two steps to getting data: unpacking the zipfiles and generating 
rawiv from DICOM.

#### Unpack ZIP

Unpacking the zipfiles is done with the `build_data_tree` script.

The format for this script is:

```
build_data_tree source_dir work_dir
```

This first copies the tree in `source_dir` to `work_dir`, to avoid potentially pollution the source data. It then traverses the file tree rooted at `work_dir`, and attempts to unzip every `.zip` file in that tree. For every ZIP it encounters, it creates four subfolders, described in the zipextract/README.md.

It also creates a directory named patient_links in the same directory as `work_dir` (that is, in `$(dirname work_dir)`). These allow you to rapidly view the patients instead of traversing the directory tree.

To emphasize: while the quick start shows you how to process the entire Box directory at once, the only requirement for `source_dir` are that it is a directory with .zip files representing DICOM data somewhere below it in the file system tree.

(Warnings about unable to read pixmap can be ignore unless there's over 10
of them for a single patient.)

#### Make RAWIV

The second step (rawiv generation) is accomplished with the `convert_all` script.
Simply pass it the work directory (the second argument to `build_data_tree`)
and it will generate rawiv and metadata JSONs.

The rawIV files will go to `VOLIMG` and the metadata to `METADATA` for each
patient, as described in the `zipextract` README.
