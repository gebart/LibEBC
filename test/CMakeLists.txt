# Create test binary
add_executable(ebctest
  src/Main.cpp
  src/TestBinaryMetadata.cpp
  src/TestBitcodeArchive.cpp
  src/TestBitcodeContainer.cpp
  src/TestBitcodeMetadata.cpp
  src/TestBitcodeUtil.cpp
  src/TestEmbeddedBitcode.cpp
  src/TestUUID.cpp
  src/TestXmlUtil.cpp
)
target_link_libraries(ebctest ebc)
target_include_directories(ebctest PRIVATE "catch")

# Use C++14 for tests
set_property(TARGET ebctest PROPERTY CXX_STANDARD 14)

add_test(ebctest ebctest)
