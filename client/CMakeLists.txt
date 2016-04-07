cmake_minimum_required(VERSION 2.8.11)

# Options
option(BUILD_TESTS "Build tests" ON)

# Dependencies part one (strict at the moment, will become optional later)
add_subdirectory(backends/libqmatrixclient)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Boost+RapidJson (aka rapidboost) flavour
#INCLUDE_DIRECTORIES(../deps/rapidjson/include)
#INCLUDE_DIRECTORIES(../deps/boost/asio/include)

# Common stuff for the whole protocol
include_directories(../common)

set(clientlib_SRCS
    client-common.cpp
    )

add_library(matrixx-client ${clientlib_SRCS})

# Tweak general compiler flags here
CHECK_CXX_COMPILER_FLAG("-Wall" WALL_FLAG_SUPPORTED)
if ( WALL_FLAG_SUPPORTED )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif ( WALL_FLAG_SUPPORTED )

# If we have CMake without target_compile_features() commands,
# try to bluntly apply a -std= flag to compilers that support it.
# If the compiler doesn't know -std= (hello, Visual Studio), then just hope
# it's not old enough (if it is, it will fail compiling anyway).
if ( CMAKE_VERSION VERSION_LESS "3.1" )
    CHECK_CXX_COMPILER_FLAG("-std=c++11" STD_FLAG_SUPPORTED)
    if ( STD_FLAG_SUPPORTED )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif ( STD_FLAG_SUPPORTED )
endif ( CMAKE_VERSION VERSION_LESS "3.1" )

# The below will be applied only to the library, not to the tests
if ( NOT CMAKE_VERSION VERSION_LESS "3.1" )
    target_compile_features(matrixx-client PUBLIC cxx_generalized_initializers)
endif ()

# Dependencies part two
target_link_libraries(matrixx-client qmatrixclient)

if (BUILD_TESTS)
    find_package(Qt5Core 5.3.0)

    # CS library tests
    set(client_testsuite_SRCS
        tests/client-testsuite.cpp
        )

    add_executable(client-testsuite ${client_testsuite_SRCS})

    if ( NOT CMAKE_VERSION VERSION_LESS "3.1" )
        target_compile_features(client-testsuite PRIVATE cxx_auto_type)
    endif ()

    target_link_libraries(client-testsuite matrixx-client)
endif (BUILD_TESTS)

