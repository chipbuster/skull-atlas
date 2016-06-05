cmake_minimum_required (VERSION 2.8)

# Find_CGAL should be in module path from parent script

## This should set BOOST_THREAD_LIBRARIES, CGAL_LIBRARIES,
#                  GMP_LIBRARIES, and MPFR_LIBRARIES

find_package(CGAL)
if (NOT CGAL_FOUND)
  message( FATAL_ERROR "CGAL or one of its supporting libraries (MPFR, GMP, Boost)\
           was not found. Please make sure CGAL is installed and working on your system.")
endif()

#add_executable(test_ach2 array_convex_hull_2.cpp)
add_executable(test_pas points_and_segments.cpp)
add_executable(test_sup surprising.cpp)

target_link_libraries(test_pas ${CGAL_LIBRARIES})
target_link_libraries(test_sup ${CGAL_LIBRARIES})

install (TARGETS test_pas test_sup DESTINATION bin)
