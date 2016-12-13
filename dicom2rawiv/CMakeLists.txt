option(BUILD_GDCM "Build GDCM. Slow compile, but needed to extract images from DICOM files" ON)

add_subdirectory(file_convert)
add_subdirectory(imgutil)
add_subdirectory(zipextract)
add_subdirectory(skull-rastering)

