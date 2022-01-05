cmake_minimum_required(VERSION 3.10)

project(Kaleidoscope)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES src/AST.cpp src/Lexer.cpp src/Parser.cpp)
set(TESTS test/test_main.cpp 
    test/test_traverse.cpp 
    test/test_lexer.cpp 
    test/test_ast.cpp
    test/test_parser.cpp)
include_directories(src)

add_compile_options(-Wall -Wextra -pedantic -Werror)

#option(PRINT_UNITTEST_TREES "Print the parse trees generated in unit tests" OFF)

if(PRINT_UNITTEST_TREES)
    message("Unit test trees will be printed to stdout")
    add_definitions(-DKALEIDISCOPE_TESTS_PRINT_TREES)
endif()

find_package(Catch2 REQUIRED)
add_executable(test ${TESTS} ${SOURCES})
target_link_libraries(test PRIVATE pthread Catch2::Catch2)
