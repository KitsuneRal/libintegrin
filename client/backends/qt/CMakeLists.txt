cmake_minimum_required(VERSION 3.1)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (DEPS_DIR ../../../deps)

find_package(Qt5Core 5.3.0) # For JSON (de)serialization
find_package(Qt5Network 5.3.0) # For networking
# Whether to build with the bundled KCoreAddons or system KCoreAddons
set( BUNDLE_KCOREADDONS "AUTO" CACHE STRING "Build own KCoreAddons, one of ON, OFF and AUTO" )

if ( (NOT BUNDLE_KCOREADDONS STREQUAL "ON")
     AND (NOT BUNDLE_KCOREADDONS STREQUAL "OFF")
     AND (NOT BUNDLE_KCOREADDONS STREQUAL "AUTO") )
       message( FATAL_ERROR "BUNDLE_KCOREADDONS must be one of ON, OFF or AUTO" )
endif ()

if ( BUNDLE_KCOREADDONS STREQUAL "AUTO" )
    find_package(KF5CoreAddons QUIET)
elseif ( BUNDLE_KCOREADDONS STREQUAL "OFF" )
    find_package(KF5CoreAddons REQUIRED)
endif ()

# Set up source files
set(matrixx_client_qt_SRCS
   connectiondata.cpp
   connection.cpp
   connectionprivate.cpp
   room.cpp
   user.cpp
   logmessage.cpp
   state.cpp
   events/event.cpp
   events/roommessageevent.cpp
   events/roomnameevent.cpp
   events/roomaliasesevent.cpp
   events/roomcanonicalaliasevent.cpp
   events/roommemberevent.cpp
   events/roomtopicevent.cpp
   events/typingevent.cpp
   events/unknownevent.cpp
   jobs/basejob.cpp
   jobs/checkauthmethods.cpp
   jobs/passwordlogin.cpp
   jobs/postmessagejob.cpp
   jobs/joinroomjob.cpp
   jobs/leaveroomjob.cpp
   jobs/roommembersjob.cpp
   jobs/roommessagesjob.cpp
   jobs/syncjob.cpp
   jobs/mediathumbnailjob.cpp
    )
# Add bundled KCoreAddons sources if we haven't found the system sources
# or if we ignore them
if ( NOT KF5CoreAddons_FOUND )
    set (matrixx_client_qt_SRCS ${matrixx_client_qt_SRCS}
        ${DEPS_DIR}/kcoreaddons/src/lib/jobs/kjob.cpp
        ${DEPS_DIR}/kcoreaddons/src/lib/jobs/kcompositejob.cpp
        ${DEPS_DIR}/kcoreaddons/src/lib/jobs/kjobtrackerinterface.cpp
        ${DEPS_DIR}/kcoreaddons/src/lib/jobs/kjobuidelegate.cpp
        )
endif ( NOT KF5CoreAddons_FOUND )

add_library(matrixx-client-qt ${matrixx_client_qt_SRCS})

target_compile_features(matrixx-client-qt PRIVATE cxx_range_for)
target_compile_features(matrixx-client-qt PRIVATE cxx_override)
target_compile_features(matrixx-client-qt PRIVATE cxx_variadic_templates)
target_compile_features(matrixx-client-qt PRIVATE cxx_lambdas)

target_link_libraries(matrixx-client-qt Qt5::Core Qt5::Network)
if ( KF5CoreAddons_FOUND )
    # The proper way of doing things would be to make a separate config.h.in
    # file and use configure_file() command here to generate config.h with
    # needed C++ preprocessor macros. If we have more than one or two
    # dependencies like that, we should turn to that more scalable way.
    # As for now, passing a macro through -D is easier to observe and maintain.
    target_compile_definitions ( matrixx-client-qt PRIVATE USING_SYSTEM_KCOREADDONS )
    target_link_libraries(KF5::CoreAddons)
else ( KF5CoreAddons_FOUND )
    include_directories( ${DEPS_DIR}/kcoreaddons/src/lib/jobs )
endif ( KF5CoreAddons_FOUND )

message( STATUS "Building with: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}" )
message( STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}" )
message( STATUS "Path to Qt Core: ${Qt5Core_DIR}" )
message( STATUS "Build own KCoreAddons (BUNDLE_KCOREADDONS): ${BUNDLE_KCOREADDONS}" )
if ( NOT BUNDLE_KCOREADDONS STREQUAL "ON" )
    if ( KF5CoreAddons_FOUND )
        message( STATUS "'- Path to system KCoreAddons: ${KF5CoreAddons_DIR}" )
    else ( KF5CoreAddons_FOUND )
        message( STATUS "'- System KCoreAddons not found, using the bundled version" )
    endif ( KF5CoreAddons_FOUND )
endif ( NOT BUNDLE_KCOREADDONS STREQUAL "ON" )
