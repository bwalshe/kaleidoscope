cmake_minimum_required(VERSION 3.10)

project(Kaleidoscope)

set(CMAKE_CXX_STANDARD 17)

set(TESTS test/test_main.cpp test/test_traverse.cpp)
include_directories(src)

find_package(Catch2 REQUIRED)
add_executable(test ${TESTS})
target_link_libraries(test PRIVATE pthread Catch2::Catch2)
