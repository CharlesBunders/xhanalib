cmake_minimum_required(VERSION 3.15)

project("xhanalib" VERSION 1.0.0
	DESCRIPTION "A header only library providing a toolbox for fast prototyping, rudimentary fuzz testing and random data generation."
	HOMEPAGE_URL "https://github.com/CharlesBunders/xhanalib"
	LANGUAGES CXX)

include(GNUInstallDirs)

add_library(xhanalib INTERFACE)

target_include_directories(
	xhanalib
	INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
            
target_compile_features(xhanalib INTERFACE cxx_std_17)

install(DIRECTORY include/ DESTINATION include)

install(TARGETS xhanalib EXPORT xhanalibTargets)
install(EXPORT xhanalibTargets
        FILE xhanalibTargets.cmake
        NAMESPACE xhanalib::
        DESTINATION lib/cmake/xhanalib)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    xhanalibConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/xhanalibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/xhanalibConfig.cmake
    INSTALL_DESTINATION lib/cmake/xhanalib
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/xhanalibConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/xhanalibConfigVersion.cmake
    DESTINATION lib/cmake/xhanalib
)
