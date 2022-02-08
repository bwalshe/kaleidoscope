cmake_minimum_required(VERSION 3.10)

project(Kaleidoscope)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES src/AST.cpp src/Lexer.cpp src/Parser.cpp src/CodegenVisitor.cpp)
set(TESTS test/test_main.cpp 
    test/test_traverse.cpp 
    test/test_lexer.cpp 
    test/test_ast.cpp
    test/test_parser.cpp
    test/test_codegen.cpp)
include_directories(src)

add_compile_options(-Wall -Werror)

#option(PRINT_UNITTEST_TREES "Print the parse trees generated in unit tests" OFF)

if(PRINT_UNITTEST_TREES)
    message("Unit test trees will be printed to stdout")
    add_definitions(-DKALEIDOSCOPE_TESTS_PRINT_TREES)
endif()


find_package(LLVM 13.0.0 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs support core irreader)

find_package(Catch2 REQUIRED)
add_executable(test ${TESTS} ${SOURCES})
target_link_libraries(test PRIVATE pthread Catch2::Catch2 ${llvm_libs})

add_executable(kaelidoscopep src/PrintIR.cpp ${SOURCES})
target_link_libraries(kaelidoscopep PRIVATE ${llvm_libs})
