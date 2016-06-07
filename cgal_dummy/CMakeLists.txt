cmake_minimum_required (VERSION 2.8)

# Find_CGAL should be in module path from parent script

## This should set BOOST_THREAD_LIBRARIES, CGAL_LIBRARIES,
#                  GMP_LIBRARIES, and MPFR_LIBRARIES

find_package(CGAL
            REQUIRED)

find_package(Boost
             REQUIRED
             thread-mt)

#add_executable(test_ach2 array_convex_hull_2.cpp)
add_executable(test_pas points_and_segments.cpp)
add_executable(test_sup surprising.cpp)
add_executable(test_ext exact.cpp)
add_executable(test_ach array_convex_hull_2.cpp)
add_executable(test_vch vector_convex_hull_2.cpp)

target_link_libraries(test_pas ${CGAL_LIB})
target_link_libraries(test_sup ${CGAL_LIB})
target_link_libraries(test_ext ${CGAL_LIB} ${MPFR_LIBRARIES} ${GMP_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(test_ach ${CGAL_LIB} ${MPFR_LIBRARIES} ${GMP_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(test_vch ${CGAL_LIB} ${MPFR_LIBRARIES} ${GMP_LIBRARIES} ${Boost_LIBRARIES})

install (TARGETS test_pas test_sup test_ext test_ach test_vch DESTINATION bin)
