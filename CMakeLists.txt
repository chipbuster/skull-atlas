cmake_minimum_required (VERSION 3.2.0)
project (Skull_Atlas)

# I don't know how to do this yet. ExternalProject trggers an install step,
# which doesnt' work if prefix is not specified. Add_subdirectory breaks
# build because relative include paths get messed up (seems like 
# maintainers were not expecting this to be a subproject)

# Need to read http://stackoverflow.com/q/37838786/2914377 and implement.

# add_subdirectory("${PROJECT_SOURCE_DIR}/extdeps")

# Locate custom find modules (e.g. CGAL)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

# Set output directories so that the build directory is NEAT UND TIDY
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_CGAL_MESH_GEN "Builds the CGAL-based mesh creation libraries" ON)
if(BUILD_CGAL_MESH_GEN)
  add_subdirectory(cgal_mesh_generation)
  if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extdeps/libigl/README.md")
    message(WARNING "You have requested C++ components, but you did not appear
to have cloned libigl. This is likely a mistake unless you have a system
copy of libigl--please see the README.md for instructions on correct cloning.")
  endif(NOT EXISTS "${PROJECT_SOURCE_DIR}/extdeps/libigl/README.md")
endif(BUILD_CGAL_MESH_GEN)

option(BUILD_DICOM_PIPELINE "Builds the pipeline to take raw DICOM images to rawiv files + metadata" ON)
if(BUILD_DICOM_PIPELINE)
  add_subdirectory(dicom2rawiv)
endif(BUILD_DICOM_PIPELINE)

option(BUILD_GLOBAL_DRIVER "Installs the scripts to drive the global pipeline." ON)
if(BUILD_GLOBAL_DRIVER)
  add_subdirectory(globalpipeline)
endif(BUILD_GLOBAL_DRIVER)
