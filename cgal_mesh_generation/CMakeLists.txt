cmake_minimum_required (VERSION 2.8)

find_package(CGAL REQUIRED COMPONENTS Core ImageIO
             PATHS "/org/centers/cvc/software/c7/libraries/CGAL4.9/")
include(${CGAL_USE_FILE})

add_subdirectory(volume_diff)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(inr2obj main.cpp)
target_link_libraries(inr2obj ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
install (TARGETS inr2obj DESTINATION bin)

add_executable(inr2mc minecraft_surf.cpp)
target_link_libraries(inr2mc ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
install (TARGETS inr2mc DESTINATION bin)

# This will convert a surface to the broken-apart-objects format for use in the
# GVD approach. We actually don't need it anymore.
add_executable(surf2medax surface_medax.cpp)
target_link_libraries(surf2medax ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
install (TARGETS surf2medax DESTINATION bin)

# Segment library uses libigl. Findlibigl is in cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Can use libigl found at /net/electron/workspace/nclement/software/libigl/build
# or download using:  git clone --recursive https://github.com/libigl/libigl.git
find_package(libigl REQUIRED)
if (LIBIGL_FOUND)
  ### libIGL options: choose between header only and compiled static library
  # Header-only is preferred for small projects. For larger projects the static build
  # considerably reduces the compilation times
  option(LIBIGL_USE_STATIC_LIBRARY "Use LibIGL as static library" OFF)

  ### add a customizable menu bar
  option(LIBIGL_WITH_NANOGUI     "Use Nanogui menu"   ON)

  ### libIGL options: choose your dependencies (by default everything is OFF except opengl) ###
  option(LIBIGL_WITH_VIEWER      "Use OpenGL viewer"  ON)
  option(LIBIGL_WITH_OPENGL      "Use OpenGL"         ON)
  option(LIBIGL_WITH_GLFW        "Use GLFW"           ON)
  option(LIBIGL_WITH_BBW         "Use BBW"            OFF)
  option(LIBIGL_WITH_EMBREE      "Use Embree"         ON)
  option(LIBIGL_WITH_PNG         "Use PNG"            OFF)
  option(LIBIGL_WITH_TETGEN      "Use Tetgen"         ON)
  option(LIBIGL_WITH_TRIANGLE    "Use Triangle"       ON)
  option(LIBIGL_WITH_XML         "Use XML"            ON)
  option(LIBIGL_WITH_LIM         "Use LIM"            OFF)
  option(LIBIGL_WITH_COMISO      "Use CoMiso"         OFF)
  option(LIBIGL_WITH_BOOLEAN     "Use Cork boolean"   OFF) # This option is not supported yet
  option(LIBIGL_WITH_MATLAB      "Use Matlab"         OFF) # This option is not supported yet
  option(LIBIGL_WITH_MOSEK       "Use MOSEK"          OFF) # This option is not supported yet
  option(LIBIGL_WITH_CGAL        "Use CGAL"           OFF)
  if(LIBIGL_WITH_CGAL) # Do not remove or move this block, the cgal build system fails without it
    find_package(CGAL REQUIRED)
    set(CGAL_DONT_OVERRIDE_CMAKE_FLAGS TRUE CACHE BOOL "CGAL's CMAKE Setup is super annoying ")
    include(${CGAL_USE_FILE})
  endif()

  ### Adding libIGL: choose the path to your local copy libIGL ###
  ### This is going to compile everything you requested ###
  #message(FATAL_ERROR "${PROJECT_SOURCE_DIR}/../libigl/cmake")
  set(LIBIGL_CMAKE "${LIBIGL_INCLUDE_DIR}/../shared/cmake" CACHE PATH "Path for libigl cmake folder")
  add_subdirectory("${LIBIGL_CMAKE}" "libigl")

  ### ligIGL information ###
  message("libIGL includes: ${LIBIGL_INCLUDE_DIRS}")
  message("libIGL libraries: ${LIBIGL_LIBRARIES}")
  message("libIGL extra sources: ${LIBIGL_EXTRA_SOURCES}")
  message("libIGL extra libraries: ${LIBIGL_EXTRA_LIBRARIES}")
  message("libIGL definitions: ${LIBIGL_DEFINITIONS}")

  ### Prepare the build environment
  include_directories(SYSTEM ${LIBIGL_INCLUDE_DIRS})
  add_definitions(${LIBIGL_DEFINITIONS})

  ### Add executables
  # Segmentation program
  add_executable(segment cgal_segment.cpp ${LIBIGL_EXTRA_SOURCES})
  target_link_libraries(segment
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS segment DESTINATION bin)

  # Removal of non-body elements.
  add_executable(precurate remove_nonbody.cpp ${LIBIGL_EXTRA_SOURCES})
  target_link_libraries(precurate
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS precurate DESTINATION bin)

  # This will convert a non-triangular .off file to a triangular .off file.
  add_executable(triangular_mesh triangular_mesh.cpp)
  target_link_libraries(triangular_mesh
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS triangular_mesh DESTINATION bin)

  # Extracts components from a mesh (will do it visually)
  add_executable(select_components select_components.cpp ${LIBIGL_EXTRA_SOURCES})
  target_link_libraries(select_components
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS select_components DESTINATION bin)

  # Sets points on the skull
  add_executable(point_selector point_selector.cpp ${LIBIGL_EXTRA_SOURCES})
  target_link_libraries(point_selector
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS point_selector DESTINATION bin)
  # Sets points on the skull
  add_executable(point_selector_raw point_selector_raw.cpp ${LIBIGL_EXTRA_SOURCES})
  target_link_libraries(point_selector_raw
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS point_selector_raw DESTINATION bin)

  # Decimate mesh
  add_executable(decimate decimate.cpp ${LIBIGL_EXTRA_SOURCES})
  target_link_libraries(decimate
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS decimate DESTINATION bin)

  # Color the mesh by the curvature values
  add_executable(color_curvature color_curvature.cpp ${LIBIGL_EXTRA_SOURCES})
  target_link_libraries(color_curvature 
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS color_curvature DESTINATION bin)

  # Distance between two meshes
  add_executable(distMeshMesh distMeshMesh.cpp ${LIBIGL_EXTRA_SOURCES})
  target_link_libraries(distMeshMesh
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS distMeshMesh DESTINATION bin)
elseif(LIBIGL_FOUND)
  message(FATAL_ERROR "You have requested components that rely on libIGL but it was not found.
If you are not developing this library, you may have cloned this git repository without
submodules. Check that extdeps/libigl actually has files inside of it, and follow the
")
endif(LIBIGL_FOUND)
