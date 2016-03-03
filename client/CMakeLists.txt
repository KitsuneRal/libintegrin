cmake_minimum_required(VERSION 3.1)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Common stuff for the whole protocol
include_directories(common)

set(clientlib_SRCS
    client-common.cpp
    )

add_library(libmatrixx-client ${clientlib_SRCS})

# Tweak general compiler flags here
set(CMAKE_CXX_STANDARD 11)
if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options( libmatrixx-client PRIVATE "-Wall -Wextra -pedantic" )
endif ( CMAKE_COMPILER_IS_GNUCC )
