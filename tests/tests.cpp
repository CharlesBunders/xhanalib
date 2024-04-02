#include <chrono>
#include <thread>
#include "xhanalib.h"
#include "acutest.h"  // AccuTest test framework https://github.com/mity/acutest/tree/master

namespace xl = xhanalib;

//
// Compile and run:
// g++ -std=c++17 -I../third_party/ test.cpp -o test.exe && ./test.exe
// Shake out warnings:
// g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -fmax-errors=1 -I../third_party/ test.cpp -o test.exe
// List tests:
// ./test.exe -l
// Run tests matching text:
// ./test.exe --time --verbose get_current_timestamp
// Run specific test:
// ./test.exe --time --verbose "get_current_timestamp() 2 - delta"
//

// Logging helper
void test_logging(void)
{
    xl::log("The value is:", "1");
    TEST_CHECK( 1 == 1 );  // If it ran, it is ok
}

// Helper to get host platform
void test_platform_name(void)
{
    auto a = xl::get_platform_name();
    TEST_CHECK_( strlen(a) > 0, "-> platform:[%s]", a );
}

// Numeric value to a string
void test_to_string_1(void)
{
    auto a = xl::to_string(1);
    auto b = xl::to_string(1);
    TEST_CHECK( a == b );
}

// String numeric to string
void test_to_string_2(void)
{
    auto a = xl::to_string("1");
    auto b = xl::to_string("1");
    TEST_CHECK( a == b );
}

// Decimal place value to string
void test_to_string_3(void)
{
    auto a = xl::to_string(1.2);
    auto b = xl::to_string(1.2);
    TEST_CHECK( a == b );
}

// Helper for simple deserialize
void test_deserialize_key_value_1(void)
{
    auto key_val_str = "name=john&age=50";
    std::map<std::string, std::string> out_map{};
    auto worked = xl::deserialize_key_value(key_val_str, '=', '&', out_map);
        
    TEST_CHECK( out_map.size() == 2 && worked);
}

// Helper for simple deserialize
void test_deserialize_key_value_2(void)
{
    auto key_val_str = "name=john&age=50";
    std::map<std::string, std::string> out_map{};
    auto worked = xl::deserialize_key_value(key_val_str, '=', '&', out_map);

    TEST_CHECK( out_map["name"] == "john" );
    TEST_CHECK( out_map["age"] == "50" );
    TEST_CHECK( worked );
}

void test_random_number_of_length_n_1(void)
{
    auto test_length = 9;
    auto a = xl::random_number_of_length_n<int>(test_length);
    TEST_CHECK_( xl::count_digits(a) == test_length, "-> num:[%d] len:[%d]", a, test_length );
    TEST_MSG("Length: %d", test_length);  // only prints on failure
}

// Too big, throws std::out_of_range exception
void test_random_number_of_length_n_2(void)
{
    auto test_length = 2147483647;
    TEST_EXCEPTION(xl::random_number_of_length_n<int>(test_length), std::out_of_range);
    TEST_MSG("Length: %d", test_length);  // only prints on failure
}

void test_random_number_of_length_n_3(void)
{
    auto test_length = 2147483649;
    //xl::log("len", test_length);
    TEST_EXCEPTION(xl::random_number_of_length_n<int>(test_length), std::out_of_range);
    TEST_MSG("Length: %ld", test_length);  // only prints on failure
}

// Single digit 1-9, requests do not return 0 as first digit
void test_random_number_of_length_n_4(void)
{
    size_t test_length = 1;
    auto rnd_num = xl::random_number_of_length_n<size_t>(test_length);
    TEST_CHECK_( xl::count_digits(rnd_num) == test_length, "-> num:[%ld]", rnd_num );
    TEST_CHECK( xl::count_digits(rnd_num) == test_length );
    TEST_MSG("Length: %ld", test_length);  // only prints on failure
}

// Random string longer than the set of char's to choose from
void test_random_string_of_length_n_1(void)
{
    size_t test_length = 10;
    auto rnd_str = xl::random_string_of_length_n(test_length, "abcd");
    TEST_CHECK_( rnd_str.length() == test_length, "-> num:[%s]", rnd_str.c_str() );
    TEST_MSG("Length: %ld", test_length);  // only prints on failure
}

// Random string shorter than the set of char's to choose from
void test_random_string_of_length_n_2(void)
{
    size_t test_length = 4;
    auto rnd_str = xl::random_string_of_length_n(test_length, "abcdefghijklmnop");
    TEST_CHECK_( rnd_str.length() == test_length, "-> num:[%s]", rnd_str.c_str() );
    TEST_MSG("Length: %ld", test_length);  // only prints on failure
}

// Random string same length as the set of char's to choose from
void test_random_string_of_length_n_3(void)
{
    size_t test_length = 4;
    auto rnd_str = xl::random_string_of_length_n(test_length, "abcd");
    TEST_CHECK_( rnd_str.length() == test_length, "-> num:[%s]", rnd_str.c_str() );
    TEST_MSG("Length: %ld", test_length);  // only prints on failure
}

void test_random_integer_from_range_x_to_y_1(void)
{
    auto a = xl::random_integer_from_range_x_to_y<int>(5, 9);
    TEST_CHECK_( a > 3 && a < 10, "-> num:[%d]", a );
    TEST_MSG("Invalid: %d", a);  // only prints on failure
}

void test_random_integer_from_range_x_to_y_2(void)
{
    auto a = xl::random_integer_from_range_x_to_y<int>(1, 19);
    TEST_CHECK_( a > 0 && a < 20, "-> num:[%d]", a );
    TEST_MSG("Invalid: %d", a);  // only prints on failure
}

void test_random_integer_from_range_x_to_y_3(void)
{
    auto a = xl::random_integer_from_range_x_to_y<int>(1000, 9999);
    TEST_CHECK_( a > 999 && a < 10000, "-> num:[%d]", a );
    TEST_MSG("Invalid: %d", a);  // only prints on failure
}

void test_random_integer_from_range_x_to_y_4(void)
{
    auto a = xl::random_integer_from_range_x_to_y<int>(10, 10);
    TEST_CHECK_( a > 9 && a < 11, "-> num:[%d]", a );
    TEST_MSG("Invalid: %d", a);  // only prints on failure
}

// Random real num (float)
void test_random_real_from_range_x_to_y_1(void)
{
    auto a = xl::random_real_from_range_x_to_y<float>(3.2, 14.777);
    TEST_CHECK_( a > 3.0 && a < 15.0, "-> num:[%f]", a );
    TEST_MSG("Invalid: %f", a);  // only prints on failure
}

// Random real num (double)
void test_random_real_from_range_x_to_y_2(void)
{
    auto a = xl::random_real_from_range_x_to_y<double>(3.2, 14.777);
    TEST_CHECK_( a > 3.0 && a < 15.0, "-> num:[%f]", a );
    TEST_MSG("Invalid: %f", a);  // only prints on failure
}

// Random real num (long double)
void test_random_real_from_range_x_to_y_3(void)
{
    auto a = xl::random_real_from_range_x_to_y<long double>(3.2, 14.777);
    TEST_CHECK_( a > 3.0 && a < 15.0, "-> num:[%Lf]", a );
    TEST_MSG("Invalid: %Lf", a);  // only prints on failure
}

// Simple timestamp return
void test_get_current_timestamp_1(void)
{
    auto a = xl::get_current_timestamp();
    TEST_CHECK_( a.length() == 12, "-> timestamp value:[%s]", a.c_str() );
    TEST_MSG("Timestamp wrong length: %s", a.c_str());  // only prints on failure
}

// Timestamp, sleep for a bit, timestamp
void test_get_current_timestamp_2(void)
{
    auto a = xl::get_current_timestamp();
    TEST_CHECK_( a.length() == 12, "-> timestamp value 1:[%s]", a.c_str() );

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    auto b = xl::get_current_timestamp();
    TEST_CHECK_( b.length() == 12, "-> timestamp value 2:[%s]", b.c_str() );

    TEST_CHECK( a != b );
    TEST_MSG("Timestamps should be different: %s %s", a.c_str(), b.c_str());  // only prints on failure
}

// Binary of a number as const char *
void test_number_as_binary_1(void)
{
    auto a = 2;
    auto b = xl::number_as_binary<int>(a);
    TEST_CHECK_( strcmp( b, "0000000000000010" ) == 0, "-> binary:[%s]", b );
    TEST_MSG("Num as binary (half): %s", b);  // only prints on failure
}

// Binary of a number as const char *, full set
void test_number_as_binary_2(void)
{
    auto a = 2;
    auto b = xl::number_as_binary<int>(a, false);
    TEST_CHECK_( strcmp( b, "00000000000000000000000000000010" ) == 0, "-> binary:[%s]", b );
    TEST_MSG("Num as binary (full): %s", b);  // only prints on failure
}

void test_execute_1(void)
{
    auto a = xl::execute("dir");
    TEST_CHECK( a.length() > 10 );
    TEST_MSG("Execute output: %s", a.c_str());  // only prints on failure
}

void test_execute_2(void)
{
    auto a = xl::execute("dir");
    TEST_CHECK( a.length() > 10 );
    TEST_MSG("Execute output: %s", a.c_str());  // only prints on failure
}

// struct xl::keyval results_case_opts[] = {
//    {0, "upper"},
//    {1, "lower"},
//    {2, "mixed"}
// };
// Usage:
// results_case_opts[p_case].value
void test_keyval_1(void)
{
    struct xl::keyval results_case_opts[] = {
       {0, "upper"},
       {1, "lower"},
       {2, "mixed"}
    };
    TEST_CHECK_( strcmp(results_case_opts[1].value, "lower") == 0, "-> results_case_opts[1].value:[%s]", results_case_opts[1].value );
}

void test_keyval_2(void)
{
    struct xl::keyval results_case_opts[] = {
       {0, "upper"},
       {1, "lower"},
       {2, "mixed"}
    };
    //xl::log("val of strcmp", strcmp(results_case_opts[4].value, "lower"));

#ifdef _WIN32
    // Invalid key on Windows
    TEST_CHECK(results_case_opts[4].value != NULL);
#else
    // Invalid key doesn't throw, FYI
    TEST_CHECK_(strcmp(results_case_opts[4].value, "lower") > 0, "-> results_case_opts[1].value:[%s]", results_case_opts[4].value);
#endif
}

void test_equal_to_n_decimal_places_1(void)
{
    TEST_CHECK( xl::equal_to_n_decimal_places( 94.257f, 94.257f, 2) == true );
}

void test_equal_to_n_decimal_places_2(void)
{
    TEST_CHECK( xl::equal_to_n_decimal_places( 94.257343432f, 94.257f, 3) == true );
}

void test_equal_to_n_decimal_places_3(void)
{
    TEST_CHECK( xl::equal_to_n_decimal_places( 94.25f, 94.26f, 2) == false );
}

// https://github.com/mity/acutest/tree/master
// cmake --build . && ctest -C Debug -V

// ./test.exe -l
// ./test.exe --verbose get_current_timestamp

TEST_LIST = {
    { "log()", test_logging },
    { "platform_name()", test_platform_name },
    { "to_string() 1", test_to_string_1 },
    { "to_string() 2", test_to_string_2 },
    { "to_string() 3", test_to_string_3 },
    { "deserialize_key_value() 1", test_deserialize_key_value_1 },
    { "deserialize_key_value() 2", test_deserialize_key_value_2 },
    { "random_number_of_length_n() 1", test_random_number_of_length_n_1 },
    { "random_number_of_length_n() 2", test_random_number_of_length_n_2 },
    { "random_number_of_length_n() 3", test_random_number_of_length_n_3 },
    { "random_number_of_length_n() 4", test_random_number_of_length_n_4 },
    { "random_string_of_length_n() 1", test_random_string_of_length_n_1 },
    { "random_string_of_length_n() 2", test_random_string_of_length_n_2 },
    { "random_string_of_length_n() 3", test_random_string_of_length_n_3 },
    { "random_integer_from_range_x_to_y() 1", test_random_integer_from_range_x_to_y_1 },
    { "random_integer_from_range_x_to_y() 2", test_random_integer_from_range_x_to_y_2 },
    { "random_integer_from_range_x_to_y() 3", test_random_integer_from_range_x_to_y_3 },
    { "random_integer_from_range_x_to_y() 4", test_random_integer_from_range_x_to_y_4 },
    { "random_real_from_range_x_to_y() 1", test_random_real_from_range_x_to_y_1 },
    { "random_real_from_range_x_to_y() 2", test_random_real_from_range_x_to_y_2 },
    { "random_real_from_range_x_to_y() 3", test_random_real_from_range_x_to_y_3 },
    { "get_current_timestamp() 1 - basic", test_get_current_timestamp_1 },
    { "get_current_timestamp() 2 - delta", test_get_current_timestamp_2 },
    { "number_as_binary() 1", test_number_as_binary_1 },
    { "number_as_binary() 2", test_number_as_binary_2 },
    { "execute() 1", test_execute_1 },
    { "execute() 2", test_execute_2 },
    { "keyval_usage_1", test_keyval_1 },
    { "keyval_usage_2", test_keyval_2 },
    { "equal_to_n_decimal_places() 1", test_equal_to_n_decimal_places_1 },
    { "equal_to_n_decimal_places() 2", test_equal_to_n_decimal_places_2 },
    { "equal_to_n_decimal_places() 3", test_equal_to_n_decimal_places_3 },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
