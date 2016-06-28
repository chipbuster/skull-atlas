Data Processing Pipeline
========================

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
