cmake_minimum_required (VERSION 2.6)
project (Lexer)
# The version number.
set (Lexer_VERSION_MAJOR 1)
set (Lexer_VERSION_MINOR 0)
 
# configure a header file to pass some of the CMake settings
# to the source code
# configure_file (
#   "${PROJECT_SOURCE_DIR}/Config.h.in"
#   "${PROJECT_BINARY_DIR}/Config.h"
#   )
 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

# add the binary tree to the search path for include files
# so that we will find Config.h
include_directories("${PROJECT_BINARY_DIR}")
 
# add the executable
add_executable(Lexer lexer.cpp)

# if (USE_MYMATH)
#   include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
#   add_subdirectory (MathFunctions)
#   set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
# endif (USE_MYMATH)
 
# add the executable
