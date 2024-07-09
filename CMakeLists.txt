cmake_minimum_required(VERSION 3.14)

project(nimble CXX)

# cpp standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message(STATUS "Using Clang/Clang++ compiler")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GCC/G++ compiler")
else()
    message(WARNING "Unsupported compiler")
endif()

# compile flags
add_compile_options(-g -Wall -pedantic)

# include directory
include_directories(include)

# source and header files
file(GLOB_RECURSE CPP_SRC src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

# create executable
add_executable(nimble ${CPP_SRC} ${HEADERS})

# output directory
set_target_properties(nimble PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME nimble
)

# clean target
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/bin/nimble
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/Makefile
)

# app run target
add_custom_target(run_bin
    COMMAND nimble
    DEPENDS nimble
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
