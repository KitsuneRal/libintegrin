cmake_minimum_required(VERSION 3.1)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies part one (strict at the moment, will become optional later)
add_subdirectory(backends/qt)

# Boost+RapidJson (aka rapidboost) flavour
#INCLUDE_DIRECTORIES(../deps/rapidjson/include)
#INCLUDE_DIRECTORIES(../deps/boost/asio/include)

# Glib flavour?
#...

# Common stuff for the whole protocol
INCLUDE_DIRECTORIES(../common)

set(clientlib_SRCS
    client-common.cpp
    )

add_library(matrixx-client ${clientlib_SRCS})

# Tweak compiler flags here
target_compile_features(matrixx-client PUBLIC cxx_generalized_initializers)
if (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(matrixx-client PRIVATE -Wall -Wextra -pedantic)
endif (CMAKE_COMPILER_IS_GNUCC)

# Dependencies part two
target_link_libraries(matrixx-client matrixx-client-qt)
