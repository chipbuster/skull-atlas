project(growskull)

include_directories(../imgutil)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O1")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3 ")

add_executable(growskull growskull.cpp rawivutil.cpp)
