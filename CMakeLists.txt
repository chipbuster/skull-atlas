cmake_minimum_required (VERSION 2.8)
project (Skull_atlas)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# Locate custom find modules (e.g. CGAL)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

# Set output directories so that the build directory is NEAT UND TIDY
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(cgal_dummy)
add_subdirectory(cgal_mesh_generation)
add_subdirectory(data_processing)
