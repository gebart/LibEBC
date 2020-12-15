cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project (LibEBC)

# Version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

# Git commit hash
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CMAKE_CXX_EXTENSIONS OFF)

# Address Sanitizers
set(SANITIZE OFF CACHE BOOL "Build with sanitizers")
if(SANITIZE)
  message(STATUS "Building with sanitizers")
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
else(SANITIZE)
  message(STATUS "Building without sanitizers")
endif(SANITIZE)

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0 -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_subdirectory(lib)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(test)
endif()
option(EBC_BUILD_TOOLS "Build ebcutil tool" ON)
if(EBC_BUILD_TOOLS)
  add_subdirectory(tool)
endif()
