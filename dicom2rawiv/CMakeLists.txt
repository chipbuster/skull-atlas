option(BUILD_FILE_UTILITIES "Build utilities that interconvert between files" ON)
option(BUILD_DICOM_EXTRACTION_UTILITIES "Build programs to extract DICOM data" ON)
option(BUILD_RASTER_SKULL_EXTRACTOR "Build program to extract a rastered skull" ON)

if(BUILD_FILE_UTILITIES)
  add_subdirectory(file_convert)
endif(BUILD_FILE_UTILITIES)

if(BUILD_DICOM_EXTRACTION_UTILITIES)
  add_subdirectory(zipextract)
endif(BUILD_DICOM_EXTRACTION_UTILITIES)

if(BUILD_RASTER_SKULL_EXTRACTOR)
  add_subdirectory(skull-rastering)
endif(BUILD_RASTER_SKULL_EXTRACTOR)

