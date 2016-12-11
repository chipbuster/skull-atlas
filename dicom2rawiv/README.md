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
   * A directory named `METADATA` that contains the processed and raw metadata
   * A directory named `VOLIMG`   that contains the volumetric images (e.g. rawiv/inr)
   * A directory named `MESHES`   that contains geometry files (e.g. obj/raw/off)

   TENTATIVE SUGGESTIONS:
   - A directory named PIPELINE that holds temporary files used by the processing pipeline

Prerequisites are:
 
 * A C/C++/Fortran compiler
 * Python 3
 * [PyDICOM](http://www.pydicom.org/)
 * Numpy
 * CMake
 * [niBabel](http://nipy.org/nibabel/)

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

NB: `<path-to-source>` needs to be the top level of the skull atlas project. The
top-level cmake file provides several important variables for the rest of the 
project.

Then run `make` and `make install`.

If all has gone well, you'll have a directory with a `bin`/`lib`/`include`/`share` 
structure. All the executables will be inside `bin`.

## Running

Before you begin, make sure you have disk: the final product will amount to
over 2GB of disk space per patient.

There are two steps to getting data: unpacking the zipfiles and generating 
rawiv from DICOM.

#### Unpack ZIP

Unpacking the zipfiles is done with the `build_data_tree` script.

The format for this script is:

You can replace `<Box Directory>` with any path--as long as that path contains
`.zip` files which contain skull data, the script will happily extract them all.
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

## Developer Notes

There are four general stages in this pipeline:extraction,decompression,rawiv creation,and metadata creation.

There are three DICOM grouping levels: a *study* is what happens when the patient goes and lies in the CT machine. The doctors may take multiple scans and have the patient move around a bit--once the patient gets up and leaves the room, the study is over. (This is a slight oversimplification, but useful).
A *series* is a single scan. What we think of as a "scan" of the head is a series. Every study should have at least one series of all or most of the skull, there may be many additional series in the study. An *image* is a 2D slice of a study, and is analagous to a photo. A bunch of images form a study.

#### Extraction

DICOM input shows up from Dr. Ross as a series of ZIP files. Depending on how the PACS server was feeling that day, the zipfiles might have various structures, different nesting patterns, etc. These are handled by cases in the zip extraction script. This part of the program is handled on a case-by-case basis and is a bit hacky. Once it is done, each patient should have a directory structure as described at the top of this page, and all the DICOM images inside the zip should be in the `DICOMOBJ` directory.

#### Decompression

The [PyDICOM package](http://www.pydicom.org/) cannot yet (Summer 2016) view compressed images. Unfortunately, some of the data comes in JPEG format (this is allowed by the DICOM standard), which iscompressed. To fix this, we use `gdcmconv`, which is part of the [Grassroots DICOM package](https://sourceforge.net/projects/gdcm/), to decompress the images. At the end of this stage, uncompressed DICOM images should be stored at `DICOMUNC` directory.

Note that if, in the future, PyDICOM becomes capable of reading compressed images, this step can be dropped and `DICOMUNC` can be replaced with a symlink to `DICOMOBJ`

#### RawIV Creation

While DICOM files are inherently 2D images, rawiv files are volumetric. This means that we need to somehow "stack" the DICOM slices to make a volumetric image.

If the slices were always in a consistent direction (e.g. always starting at the top of the head and moving down) and consistently spaced, this would be a pretty easy task. Of course, we're not so lucky. Some of the issues we have to deal with are:

  * Some series have partial scans, i.e. not of the entire skull. We need some way to filter these series out. (Soln: reject any series with fewer than N images. In practice this does a pretty good job of rejecting small scans)
  * Spacing may be inconsistent--there may only be 0.5mm between pixels in the same image, but 2mm between images. We need to account for this when making the rawiv.

