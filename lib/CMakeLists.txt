cmake_minimum_required(VERSION 3.7.0)
include(CheckLibraryExists)

# LibXml2 Dependency
find_package(LibXml2 REQUIRED)

if (NOT TARGET LLVMCore)
  # LLVM Dependency
  message(STATUS "LLVM Hint: ${LLVM_HINT}")
  find_package(LLVM REQUIRED CONFIG HINTS ${LLVM_HINT})
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
endif()
include_directories(${LLVM_INCLUDE_DIRS})

if ("LLVM" IN_LIST LLVM_AVAILABLE_LIBS)
  set (EBC_LLVM_LIBRARIES
    LLVM
  )
else()
  set (EBC_LLVM_LIBRARIES
    LLVMSupport
    LLVMObject
  )
endif()

# Xar
set(BUILD_WITH_XAR ON CACHE BOOL "Build with xar support")
if(BUILD_WITH_XAR)
  find_library(XAR_LIBRARY_NAME NAMES xar)
  if(XAR_LIBRARY_NAME)
    set(HAVE_LIBXAR true)
    set(XAR_LIBRARY ${XAR_LIBRARY_NAME})
  else ()
    check_library_exists(xar xar_open "" HAVE_LIBXAR)
    if(HAVE_LIBXAR)
      set(XAR_LIBRARY xar)
    endif()
  endif()
endif()

if(HAVE_LIBXAR)
  message(STATUS "Building with libxar: " ${XAR_LIBRARY})
else()
  message(STATUS "Building without libxar")
endif()

# Configure header
configure_file (
  "${PROJECT_SOURCE_DIR}/lib/include/ebc/Config.h.in"
  "${PROJECT_BINARY_DIR}/ebc/Config.h")

# LibEBC
add_library(ebc
  src/BinaryMetadata.cpp
  src/BitcodeArchive.cpp
  src/BitcodeContainer.cpp
  src/BitcodeMetadata.cpp
  src/BitcodeRetriever.cpp
  src/EmbeddedFile.cpp
  src/EmbeddedFileFactory.cpp
  src/EmbeddedXar.cpp
  src/util/Bitcode.cpp
  src/util/MachO.cpp
  src/util/UUID.cpp
  src/util/Xar.cpp
  src/util/Xml.cpp
)
target_link_libraries(ebc
  ${UUID_LIBRARY}
  ${ICONV_LIBRARY}
  ${XAR_LIBRARY}
  ${EBC_LLVM_LIBRARIES}
  LibXml2::LibXml2)

target_include_directories(ebc PRIVATE ${PROJECT_BINARY_DIR})
target_include_directories(ebc PUBLIC include)

# Use C++14
set_property(TARGET ebc PROPERTY CXX_STANDARD 14)
