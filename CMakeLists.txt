cmake_minimum_required (VERSION 2.8)
project (Skull_atlas)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# Locate custom find modules (e.g. CGAL)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

# Set output directories so that the build directory is NEAT UND TIDY
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_CGAL_MESH_GEN "Builds the CGAL-based mesh creation libraries" ON)
if(BUILD_CGAL_MESH_GEN)
  add_subdirectory(cgal_mesh_generation)
endif(BUILD_CGAL_MESH_GEN)

option(BUILD_DICOM_PIPELINE "Builds the pipeline to take raw DICOM images to rawiv files + metadata" ON)
if(BUILD_DICOM_PIPELINE)
  add_subdirectory(dicom2rawiv)
endif(BUILD_DICOM_PIPELINE)