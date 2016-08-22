cmake_minimum_required (VERSION 2.8)

find_package(CGAL REQUIRED COMPONENTS Core)
include(${CGAL_USE_FILE})

add_executable(medial_axis main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

target_link_libraries(medial_axis ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})
