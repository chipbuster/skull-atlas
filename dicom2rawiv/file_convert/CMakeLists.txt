install(PROGRAMS convert_all.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME convert_all)

# Also install to actual install location
install(PROGRAMS dicom_to_rawiv.py DESTINATION bin
  RENAME dicom-to-rawiv)

install(PROGRAMS volimg_convert.py DESTINATION bin
  RENAME volimg-convert)
