# Add grassroots DICOM to build

add_subdirectory(gdcm-2.6.3)

# Install the shell scripts
install(build_data_tree.sh DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\
  RENAME build_data_tree PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE)
install(unzip_and_process.sh DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\
  RENAME unzip_and_process PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE)
install(find_invalid_dicom.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\
  RENAME find_invalid_dicom PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE)
install(find_nonorthogonal_dicom.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\
  RENAME find_nonorthogonal_dicom PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE)
