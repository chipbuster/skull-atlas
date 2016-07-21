cmake_minimum_required (VERSION 2.8)

add_subdirectory(volume_diff)

find_package(CGAL REQUIRED COMPONENTS Core ImageIO)
include(${CGAL_USE_FILE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(inr2obj main.cpp)
target_link_libraries(inr2obj ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
install (TARGETS inr2obj DESTINATION bin)

add_executable(inr2mc minecraft_surf.cpp)
target_link_libraries(inr2mc ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
install (TARGETS inr2mc DESTINATION bin)

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
  option(LIBIGL_WITH_EMBREE      "Use Embree"         OFF)
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
  include_directories(${LIBIGL_INCLUDE_DIRS})
  add_definitions(${LIBIGL_DEFINITIONS})

  ### Add your project files
  add_executable(segment cgal_segment.cpp ${LIBIGL_EXTRA_SOURCES})
  target_link_libraries(segment 
    ${CGAL_LIBRARIES} 
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${LIBIGL_LIBRARIES}
    ${LIBIGL_EXTRA_LIBRARIES})
  install (TARGETS segment DESTINATION bin)
endif(LIBIGL_FOUND)