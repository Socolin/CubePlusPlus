cmake_minimum_required(VERSION 2.6)

project(Server)

SET(CMAKE_MODULE_PATH CMakeScripts)

SET(BOOST_MIN_VERSION "1.49.0")
FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} REQUIRED)

FIND_PACKAGE(ZLIB REQUIRED)
IF (ZLIB_FOUND)
    MESSAGE(STATUS "Cmake FindZLIB: using ZLIB includes at: ${ZLIB_INCLUDE_DIR}")
    MESSAGE(STATUS "Cmake FindZLIB: using ZLIB libraries: ${ZLIB_LIBRARIES}")
ENDIF (ZLIB_FOUND)

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "-Wall -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -fvar-tracking -pg -Wall -Wextra -Wpedantic  -Wswitch-default -Wzero-as-null-pointer-constant -Wswitch-default -Wunsafe-loop-optimizations")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Compiler-specific C++11 activation.
execute_process(
	COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
if (NOT (GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8))
	message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.8 or greater.")
endif ()

# Define includes directories (for headers)
include_directories(
    ${Boost_INCLUDE_DIRS}
    src/
)

link_directories(
    ${Boost_LIBRARY_DIRS}
)

# Source files
file(
    GLOB_RECURSE
    source_files
    src/*
)

set(EXECUTABLE_OUTPUT_PATH bin)

add_executable(
    Server
    ${source_files}
)

# Libs
target_link_libraries(
	Server
	cryptopp
	z
	config++
	mysqlcppconn
    nbtfield
    sfml-network
    sfml-system
)

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory bin/log)
