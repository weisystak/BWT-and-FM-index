project(BWT_test)

# for BWT.hpp
include_directories(${PROJECT_SOURCE_DIR})
# for submodule suffix array
include_directories("${PROJECT_SOURCE_DIR}/include")

# copy test data
file(COPY
  "${PROJECT_SOURCE_DIR}/unit_test/test_data"
  DESTINATION "${PROJECT_BINARY_DIR}/unit_test/"
  )

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS  "-std=c++17")
add_executable(BWT_test unit_test/BWT_test.cpp)
target_link_libraries(BWT_test gtest pthread gtest_main)

