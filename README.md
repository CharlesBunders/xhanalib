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
```bash
namespace xl = xhanalib;

xl::log("The value is:", "1");
xl::log_once("The values are:", "1", "2");  // Will only print once

auto a = xl::get_platform_name();
auto a = xl::to_string(1);
auto a = xl::to_string("1");
auto a = xl::to_string(1.2);

auto key_val_str = "name=john&age=50";
std::map<std::string, std::string> out_map{};
auto w = xl::deserialize_key_value(key_val_str, '=', '&', out_map);

out_map["name"] == "john"
out_map["age"] == "50"

auto test_length = 9;
auto a = xl::random_number_of_length_n<int>(test_length);

size_t test_length = 4;
auto rnd_str = xl::random_string_of_length_n(test_length, "abcdefghijklmnop");

auto a = xl::random_integer_from_range_x_to_y<int>(5, 9);

auto a = xl::random_real_from_range_x_to_y<float>(3.2, 14.777);

auto a = xl::get_current_timestamp();

auto a = 2;
auto b = xl::number_as_binary<int>(a);

auto a = xl::execute("dir");

struct xl::keyval results_case_opts[] = {
   {0, "upper"},
   {1, "lower"},
   {2, "mixed"}
};
Usage:
  results_case_opts[p_case].value

if( xl::equal_to_n_decimal_places( 94.257343432f, 94.257f, 3) == true )
```

# Project provided by:

[Xhana Labs](https://www.xhanalabs.com) 
