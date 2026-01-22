# Initialize
# ----------
cmake_minimum_required(VERSION 3.10)
project(CS-4600)

#include(FetchContent)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Check Compiler Architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "Architecture: 64-bit detected")
else()
	message(STATUS "Architecture: 32-bit detected")
endif()

# Define executables
# ------------------

# Is this line really necessary ???? Test later
include_directories("${CMAKE_SOURCE_DIR}/include")

# Setup Dependencies
add_subdirectory(lib)

# Define libraries
add_subdirectory(library-shader)

# Define programs
add_subdirectory(program-render-test)



