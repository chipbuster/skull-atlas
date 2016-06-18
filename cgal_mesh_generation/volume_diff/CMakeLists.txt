cmake_minimum_required (VERSION 2.8)

find_package(CGAL REQUIRED COMPONENTS Core ImageIO)
include(${CGAL_USE_FILE})

add_executable(diff_volume main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

target_link_libraries(diff_volume ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

install (TARGETS diff_volume DESTINATION bin)
