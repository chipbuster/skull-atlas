install(PROGRAMS dicom_to_rawiv.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME dicom_to_rawiv)

install(PROGRAMS volimg_convert.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME volimg_convert)

#Also install to actual install location
install(PROGRAMS dicom_to_rawiv.py DESTINATION bin
  RENAME dicom_to_rawiv)

install(PROGRAMS volimg_convert.py DESTINATION bin
  RENAME volimg_convert)
