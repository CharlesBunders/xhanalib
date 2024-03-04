# xhanalib
The Xhana Labs Library (xhanalib) is a C++ header only library providing a toolbox for fast prototyping, rudimentary fuzz testing and random data generation.

## Integrating xhanalib in to a project

Xhanalib is header only. You can add the header file directly to your project.

### Example of integrating this library in to a CMake project using FetchContent

Create a sub directory in your project named dependencies.

CMakeLists.txt in the dependencies directory:
```cmake
include(FetchContent)

FetchContent_Declare(
  xhanalib
  GIT_REPOSITORY https://github.com/CharlesBunders/xhanalib.git
  GIT_TAG "main"
)

FetchContent_Declare(
  acutest
  GIT_REPOSITORY https://github.com/mity/acutest.git
  GIT_TAG "master"
)

FetchContent_MakeAvailable(xhanalib acutest)
```
Then in your main CMakeLists.txt add:
```cmake
add_subdirectory(dependencies)
```
## Test Suite

### CMake build of this repo to run the test suite
Clone the repository, then:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
To run the tests:
```bash
./tests/xhanalib_tests
```
## Library Usage Examples
TODO

# Project provided by:

[Xhana Labs](https://www.xhanalabs.com) 
