cmake_minimum_required(VERSION 3.16)
project(cpp_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app src/main.cpp)

# Тесты собираются ТОЛЬКО если BUILD_TESTS=ON
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    
    add_executable(tests tests/test_processor.cpp)
    target_link_libraries(tests GTest::gtest_main)
    add_test(NAME processor_test COMMAND tests)
endif()