/*
Copyright(c) 2022 Charles L. Bunders

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// The xhanalib.h Library
//
// The purpose of the Xhana Labs library (xhana lib) is to provide a toolbox 
// for fast prototyping, rudimentary fuzz testing and random data generation.
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#pragma once

#ifndef INCLUDE_XHANALIB_H
#define INCLUDE_XHANALIB_H

#undef max

#include <sstream>
#include <random>
#include <string>
#include <map>
#include <limits>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <array>
#include <cmath>

/**
* Determination a platform of an operation system
* Fully supported supported only GNU GCC/G++, partially on Clang/LLVM
*/

#if defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows
#elif defined(_WIN64)
#define PLATFORM_NAME "windows" // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__ANDROID__)
#define PLATFORM_NAME "android" // Android (implies Linux, so it must come first)
#elif defined(__linux__)
#define PLATFORM_NAME "linux"   // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
#include <sys/param.h>
#if defined(BSD)
#define PLATFORM_NAME "bsd" // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
#endif
#elif defined(__hpux)
#define PLATFORM_NAME "hp-ux"   // HP-UX
#elif defined(_AIX)
#define PLATFORM_NAME "aix"     // IBM AIX
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#define PLATFORM_NAME "ios" // Apple iOS
#elif TARGET_OS_IPHONE == 1
#define PLATFORM_NAME "ios" // Apple iOS
#elif TARGET_OS_MAC == 1
#define PLATFORM_NAME "osx" // Apple OSX
#endif
#elif defined(__sun) && defined(__SVR4)
#define PLATFORM_NAME "solaris" // Oracle Solaris, Open Indiana
#else
#define PLATFORM_NAME NULL
#endif

// If a test fails, enable trace logging for it to troubleshoot
// Assuming using acutest.h
namespace
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    constexpr auto &pipe_open_stream = _popen;
    constexpr auto &pipe_close_stream = _pclose;
#else
    constexpr auto &pipe_open_stream = popen;
    constexpr auto &pipe_close_stream = pclose;
#endif

    template <typename T1, typename T2>
    static void TraceLog(T1 msg, T2 val, bool enableTraceLogging = false)
    {
        if (enableTraceLogging)
        {
            std::cout << msg << ":[" << val << "]\n";
        }
    }
}

namespace xhanalib
{
    // Shortcut to print "msg", value
    // xl::log("The value is:", variable);
    template <typename T1, typename T2>
    auto log(T1 msg, T2 val) -> void
    {
        // Set to true to enable logging, false to disable
        // Implement otherwise to suit your needs
        constexpr bool enableLogging = true;

        if constexpr (enableLogging)
        {
            std::cout << msg << ":[" << val << "]\n";
        }
    }

    template <typename T1, typename T2, typename T3>
    auto log_once(T1 msg, T2 val1, T3 val2) -> void
    {
        static bool hasBeenCalled = false;
        if (!hasBeenCalled)
        {
            std::cout << msg << ":[" << val1 << "] " << "[" << val2 << "]\n";
        }
        hasBeenCalled = true;
    }
    
    // Return a name of platform, if determined, otherwise - an empty string
    auto get_platform_name() -> const char * 
    {
        return (PLATFORM_NAME == NULL) ? "" : PLATFORM_NAME;
    }

    // Used for making data structures like:
    //
    // struct xl::keyval results_case_opts[] = {
    //    {0, "upper"},
    //    {1, "lower"},
    //    {2, "mixed"}
    // };
    //
    // Usage:
    //   results_case_opts[p_case].value
    struct keyval {
        int key;
        const char *value;
    };

    // Number of digits in a number.
    //
    // Usage:
    //   auto a = xl::to_string(1);
    template <typename T>
    static T count_digits(T number) 
    {
        T count = 0;
        while(number != 0) {
            number = number / 10;
            count++;
        }
        return count;
    }

    // Float or decimal comparison to n decimal places
    //
    // Usage:
    //   if( TEST_CHECK( equalToNDecimalPlaces( c.circumference(), 94.2478f, 4) )) {
    bool equal_to_n_decimal_places(float a, float b, int decimal_places) {
        float epsilon = std::pow(10, -decimal_places);
        return std::abs(a - b) < epsilon;
    }
    
    // Type neutral way to pull strings of numerics.
    //
    // Usage:
    //   auto a = xl::to_string(1);
    //   auto a = xl::to_string("1");
    //   auto a = xl::to_string(1.2);
    template <class T>
	inline auto to_string (const T& t) -> std::string
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

    // Simple deserialize key value strings with element and item
    // separators. (like with CGI param's)
    // Usage:
    //   auto a = "name=john&age=50";
    //   std::map<std::string, std::string> m{};
    //   if (xl::deserialize_key_value(s, '=', '&', m))
    auto deserialize_key_value(const std::string& in_str,
        const char element_sep,
        const char item_sep,
        std::map<std::string, std::string>& out_map) -> bool
    {
        std::size_t begin{ 0 };
        std::size_t end{ 0 };

        while (begin < in_str.size()) {
            // Search key
            end = in_str.find(element_sep, begin);
            if (end == std::string::npos)
                return false;

            auto key = in_str.substr(begin, /*size=*/ end - begin);
            begin = end + 1;

            // Search value
            end = in_str.find(item_sep, begin);
            auto value = in_str.substr(begin, end == std::string::npos ? std::string::npos : /*size=*/ end - begin);
            begin = (end == std::string::npos) ? in_str.size() : end + 1;

            // Store key-value
            auto emplaceResult = out_map.emplace(std::move(key), std::move(value));
            if (!emplaceResult.second)
                return false;
        }
        return true;
    }
	
    // Specify an integer type and the lower and upper bound inclusive
    template <typename T1>
    auto random_integer_from_range_x_to_y( T1 lower_boundary, 
        T1 upper_boundary ) -> T1
    {
        constexpr bool traceLoggingEnabled = false;

        // Seed the random number generator with a truly random value
        std::random_device rd;

        // Create a Mersenne Twister engine and seed it with the random device
        std::mt19937 gen(rd());

        // Create a uniform_int_distribution using the desired range
        std::uniform_int_distribution<T1> distribution(lower_boundary, upper_boundary);

        // Generate a random integer using the distribution and the random number generator
        T1 result_num = distribution(gen);

        // Output the generated random integer
        TraceLog("Random number:", result_num, traceLoggingEnabled);

        return result_num;
    }
    
    // Can specify float, double or long double
    template <typename T1>
    auto random_real_from_range_x_to_y( T1 lower_boundary, 
        T1 upper_boundary) -> T1
	{
        constexpr bool traceLoggingEnabled = false;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<T1> distribution(lower_boundary, upper_boundary);
        float result_num = distribution(gen);
        TraceLog("Random number:", result_num, traceLoggingEnabled);
        return result_num;
    }
    
    // Request a random number of length n.
	// Number must be one digit shorter than max value of type.
	// Throws std::out_of_range if not shorter.
	// Ex.
	// auto a = random_number_of_length_n<int>(4);  // Get a random 4 digit integer
	template <typename T1>
	auto random_number_of_length_n( size_t length_of_number ) -> T1
	{
        constexpr bool traceLoggingEnabled = false;
		T1 return_num = 0;

		// Throw if size of type chosen isn't one digit shorter than size requested
		TraceLog("max value for (T1)", std::numeric_limits<T1>::max(), traceLoggingEnabled);
		size_t max_digits_of_type = count_digits<T1>( std::numeric_limits<T1>::max() );
		if(max_digits_of_type > length_of_number && length_of_number > 0)
		{
			// This is okay
		}
		else
		{
			throw std::out_of_range("Digits of requested number must be one less than type used.");
		}
		TraceLog("digits in type:", max_digits_of_type, traceLoggingEnabled);
		TraceLog("digits requested:", length_of_number, traceLoggingEnabled);
		
		const std::string dist_chars {"1234567890"};
		std::string::size_type length_of_rndstring = length_of_number;
        thread_local static std::mt19937_64 rg{ std::random_device{}() };
        std::string s;
        s.reserve(length_of_rndstring);
        std::uniform_int_distribution<std::string::size_type> pick(0, dist_chars.length() - 1);

        while (length_of_rndstring--)
        {
			// Need to skip 0 if it is the first one
			if((length_of_number-1)==length_of_rndstring)
			{
				do
				{
					s = dist_chars[pick(rg)];
					TraceLog("In do while", s, traceLoggingEnabled);
				} while (s == "0");
			}
			else
			{
				s += dist_chars[pick(rg)];
				TraceLog("Appended to s", s, traceLoggingEnabled);
			}
        }
		std::stringstream ss_for_num(s);
		ss_for_num >> return_num;

        return return_num;
    }

    // Pass in: Length of requested string, characters to choose from
    // https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
    auto random_string_of_length_n(std::string::size_type length_of_rndstring,
        const std::string& dist_chars) -> std::string
    {
        /*
        Pseudo-random number engines (instantiations)
        default_random_engine   Default random engine (class )
        minstd_rand             Minimal Standard minstd_rand generator (class )
        minstd_rand0            Minimal Standard minstd_rand0 generator (class )
        mt19937                 Mersenne Twister 19937 generator (class )
        mt19937_64              Mersenne Twister 19937 generator (64 bit) (class )
        ranlux24_base           Ranlux 24 base generator (class )
        ranlux48_base           Ranlux 48 base generator (class )
        ranlux24                Ranlux 24 generator (class )
        ranlux48                Ranlux 48 generator (class )
        knuth_b                 Knuth-B generator (class )
        */

        //thread_local static std::mt19937 rg{std::random_device{}()};
        thread_local static std::mt19937_64 rg{ std::random_device{}() };
        //thread_local static std::knuth_b rg{std::random_device{}()};
        //thread_local static std::ranlux48 rg{std::random_device{}()};

        // Note: Don't do thread local static on uniform_int_dist if variable length items
        //thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, dist_chars.length() - 1);
        std::uniform_int_distribution<std::string::size_type> pick(0, dist_chars.length() - 1);

        std::string s;
        s.reserve(length_of_rndstring);
        while (length_of_rndstring--)
        {
            s += dist_chars[pick(rg)];
        }

        return s;
    }

    // Return timestamp with milliseconds as a std:string
    // Format: 23:47:24.805
    auto get_current_timestamp() -> std::string
    {
        using namespace std::chrono;
        using clock = system_clock;

        const auto current_time_point{ clock::now() };
        const auto current_time{ clock::to_time_t(current_time_point) };
        const auto current_localtime{ *std::localtime(&current_time) };
        const auto current_time_since_epoch{ current_time_point.time_since_epoch() };
        const auto current_milliseconds{ duration_cast<milliseconds> (current_time_since_epoch).count() % 1000 };

        std::ostringstream stream;
        stream << std::put_time(&current_localtime, "%T") << "." << std::setw(3) << std::setfill('0') << current_milliseconds;
        return stream.str();
    }

    // Shorten just chops it in half if you only care about the right side.
    template <typename T1>
    auto number_as_binary(T1 num, bool shorten = true) -> const char *
    {
        static char buffer[sizeof(num) * 8 + 1]; // +1 for null terminator
        size_t size = sizeof(num) * 8; // Number of bits in type

        if (shorten)
        {
            size = size / 2;
            for (int i = size - 1; i >= 0; i--) {
                int bit = (num >> i) & 1;
                buffer[size - i - 1] = '0' + bit;
            }
        }
        else
        { 
            for (int i = size - 1; i >= 0; i--) {
                int bit = (num >> i) & 1;
                buffer[size - i - 1] = '0' + bit;
            }
        }

        buffer[size] = '\0'; // Null terminator

        return buffer;
    }

    // System call, return output as string
    [[nodiscard]] auto execute(const char* cmd) -> std::string 
    {
        std::string result;
        std::array<char, 128> buffer;

        std::unique_ptr<FILE, decltype(&pipe_close_stream)> pipe(pipe_open_stream(cmd, "r"), 
            pipe_close_stream);

        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        return result;
    }
}
#endif
