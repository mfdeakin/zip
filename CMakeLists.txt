
cmake_minimum_required (VERSION 3.9)
project(zip)

set(google_benchmark_path /usr/local CACHE PATH "Path to where Google Benchmark is installed")

enable_testing()

add_executable(unit_tests tests/zip_tests.cpp)
set_target_properties(unit_tests PROPERTIES COMPILE_FLAGS "-g -std=c++17 -Wall")
target_include_directories(unit_tests PUBLIC "${PROJECT_SOURCE_DIR}/include")
add_test(all unit_tests)
