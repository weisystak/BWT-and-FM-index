cmake_minimum_required(VERSION 3.10)

# for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# for header files of suffix array
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# copy test data
file(COPY
  "${CMAKE_CURRENT_SOURCE_DIR}/unit_test/test_data"
  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/unit_test/"
  )

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS  "-std=c++17")
project(Aligner_test)
add_executable(Aligner_test unit_test/Aligner_test.cpp)
target_link_libraries(Aligner_test gtest pthread gtest_main)
