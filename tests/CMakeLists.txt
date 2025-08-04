# xhanalib/tests/CMakeLists.txt

# Find all test source files (can be hardcoded or globbed)
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})

    # 🔴 THIS IS THE KEY: link xhanalib so include paths are inherited
    target_link_libraries(${TEST_NAME} PRIVATE xhanalib)

    target_compile_features(${TEST_NAME} PRIVATE cxx_std_17)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
