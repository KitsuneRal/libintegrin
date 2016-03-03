cmake_minimum_required(VERSION 3.1)

project(libmatrixx)
set (libmatrixx_VERSION_MAJOR 0)
set (libmatrixx_VERSION_MINOR 0)

# Options
option(BUILD_TESTS "Build tests" ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Common stuff for the whole protocol
include_directories(common)

# Client-server library
add_subdirectory(client)

if (BUILD_TESTS)
    # CS library tests
    set(client_testsuite_SRCS
        client-testsuite.cpp
        )

    add_executable(client-testsuite ${client_testsuite_SRCS})
    target_link_libraries(client-testsuite libmatrixx-client)
endif (BUILD_TESTS)

# Tweak general compiler flags here
set(CMAKE_CXX_STANDARD 11)
if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options( client-testsuite PRIVATE "-Wall -Wextra -pedantic" )
endif ( CMAKE_COMPILER_IS_GNUCC )
