#
# This file sets some defaults from your *source* directory, such that
# for each build directory you create, the same defaults are used.
#

#
# BIG WARNING :
#
# DO NOT EDIT THIS FILE ! Instead: copy it to orocos-ocl.cmake (in the
# top source directory) and edit that file to set your defaults.
#
# ie: cp orocos-ocl.default.cmake orocos-ocl.cmake ; edit orocos-ocl.cmake
#
# It will be included by the build system, right after the compiler has been
# detected (see top level CMakeLists.txt file)

#
# BIG WARNING :
#
# This is *NOT* a CMake Toolchain file. See http://www.paraview.org/Wiki/CMake_Cross_Compiling
# on how to create and one. This file is included *too late* in the cmake process
# to function correctly as a toolchain file. ESPECIALLY: don't set a compiler
# in this file, this will reset all your cached settings. Do not use this
# file to setup a cross-compilation environment.

#
# Uncomment to set additional include and library paths for: 
# Boost, Xerces, TAO, Omniorb etc.
#
# set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/orocos/Boost-1_36_0/include")
# set(CMAKE_LIBRARY_PATH ${CMAKE_INCLUDE_PATH} "C:/orocos/Boost-1_36_0/lib")

#
# An option to make it easy to turn on all tests (defaults to OFF)
#
# option( BUILD_TESTS "Turn me off to disable compilation of all tests" ON )

#
# Set the target operating system. One of [lxrt gnulinux xenomai macosx win32]
# You may leave this as-is or force a certain target by removing the if... logic.
#
if (MSVC)
  set( OROCOS_TARGET win32 CACHE STRING "The Operating System target. One of [lxrt gnulinux xenomai macosx win32]")
elseif( APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  set( OROCOS_TARGET macosx CACHE STRING "The Operating System target. One of [lxrt gnulinux xenomai macosx win32]")
else()
  set( OROCOS_TARGET gnulinux CACHE STRING "The Operating System target. One of [lxrt gnulinux xenomai macosx win32]")
endif()

#
# Sets the CMAKE_BUILD_TYPE to Release by default. This is not a normal
# CMake flag which is not readable during configuration time.
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# Useful for Windows/MSVC builds, sets all libraries and executables in one place.
#
# Uncomment to let output go to bin/ and libs/
#
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "Bin directory")
# set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs CACHE PATH "Lib directory")
#
# And additional link directories.
#
# LINK_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/libs )






