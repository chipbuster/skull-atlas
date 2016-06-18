cmake_minimum_required (VERSION 2.8)

add_subdirectory(volume_diff)

find_package(CGAL REQUIRED COMPONENTS Core ImageIO)
include(${CGAL_USE_FILE})

add_executable(inr2obj main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

target_link_libraries(inr2obj ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

install (TARGETS inr2obj DESTINATION bin)
