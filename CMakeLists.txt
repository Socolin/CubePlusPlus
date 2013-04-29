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
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.8 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

INCLUDE_DIRECTORIES(src/)


link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

set(EXECUTABLE_OUTPUT_PATH bin)

file(
    GLOB_RECURSE
    source_files
    src/*
)
add_executable(
    Server
    ${source_files}
)
target_link_libraries(
	Server
	cryptopp
	z
	config++
	mysqlcppconn
)
configure_file(
	server.cfg
    ${EXECUTABLE_OUTPUT_PATH}/server.cfg
	COPYONLY
)
