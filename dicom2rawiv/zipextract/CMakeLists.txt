# Add grassroots DICOM to build

if(BUILD_GDCM)
  add_subdirectory(gdcm-2.6.3)
endif(BUILD_GDCM)

# Install the shell scripts
install(PROGRAMS build_data_tree.sh DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME build_data_tree)
install(PROGRAMS unzip_and_process.sh DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME unzip_and_process)
install(PROGRAMS find_invalid_dicom.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME find_invalid_dicom)
install(PROGRAMS find_nonorthogonal_dicom.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME find_nonorthogonal_dicom)

# Also install to install location
install(PROGRAMS build_data_tree.sh DESTINATION bin
  RENAME build_data_tree)
install(PROGRAMS unzip_and_process.sh DESTINATION bin
  RENAME unzip_and_process)
install(PROGRAMS find_invalid_dicom.py DESTINATION bin
  RENAME find_invalid_dicom)
install(PROGRAMS find_nonorthogonal_dicom.py DESTINATION bin
  RENAME find_nonorthogonal_dicom)
