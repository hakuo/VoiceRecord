cmake_minimum_required(VERSION 3.2)

option(BUILD_TESTS "Enable to build unit test" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")

if(BUILD_TESTS)
    # generate *.gcno for coverage report (gcda will be generated on runtime)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# setup install dir
include(GNUInstallDirs)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

# setup output dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_INSTALL_INCLUDE_DIR})
link_directories(${CMAKE_INSTALL_LIBDIR})

add_subdirectory(src)

if(BUILD_TESTS)
    # for testing
    set(PROJECT_MAIN_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
    add_subdirectory(test)
endif()
