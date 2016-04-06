cmake_minimum_required(VERSION 3.1)

project(libmatrixx)
set (libmatrixx_VERSION_MAJOR 0)
set (libmatrixx_VERSION_MINOR 0)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Common stuff for the whole protocol
include_directories(common)

# Client-server library
add_subdirectory(client)
