install(PROGRAMS convert_all.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME convert_all)

# Also install to actual install location
install(PROGRAMS convert_all.py DESTINATION bin
  RENAME convert_all)

install(PROGRAMS volimg_convert.py DESTINATION bin
  RENAME volimg-convert)
