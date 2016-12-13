install(PROGRAMS convert_all.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME convert_all)
install(FILES dicomutil.py rawivutil.py slices2rawiv.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Also install to actual install location
install(PROGRAMS convert_all.py DESTINATION bin
  RENAME convert_all)
