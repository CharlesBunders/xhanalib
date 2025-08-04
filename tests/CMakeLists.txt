message ("Value of var1: ${CMAKE_CURRENT_BINARY_DIR}")

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/../include
  ${CMAKE_CURRENT_BINARY_DIR}/../_deps/acutest-src/include
)

add_executable(xhanalib_tests 
  tests.cpp 
)

target_compile_features(xhanalib_tests PUBLIC cxx_std_17)

# Enable compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(xhanalib_tests PRIVATE -Wall -Wextra -pedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(xhanalib_tests PRIVATE /W4 /permissive-)
endif()

target_include_directories(
  xhanalib_tests 
  PUBLIC 
  ${CMAKE_SOURCE_DIR}/include
)
