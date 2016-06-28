
install(PROGRAMS convert_all.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RENAME convert_all)
install(FILES dicomutil.py rawivutil.py slices2rawiv.py DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
