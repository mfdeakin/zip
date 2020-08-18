
cmake_minimum_required (VERSION 3.9)
project(zip)

set(google_benchmark_path /usr/local CACHE PATH "Path to where Google Benchmark is installed")

enable_testing()

add_executable(unit_tests tests/zip_tests.cpp)
set_target_properties(unit_tests PROPERTIES COMPILE_FLAGS "-g -std=c++17 -Wall")
target_include_directories(unit_tests PUBLIC "${PROJECT_SOURCE_DIR}/include")
add_test(all unit_tests)

set(BUILD_PERFORMANCE FALSE CACHE BOOL "Whether to build the performance test")
if(BUILD_PERFORMANCE)
  set(google_benchmark_path /usr/local CACHE PATH "Path to where Google Benchmark is installed")
  link_directories("${google_benchmark_path}/lib")

  add_executable(performance tests/performance.cpp)
  set(CMAKE_CXX_FLAGS_RELEASE "-march=native -fstrict-aliasing -ffast-math -O3")
  target_compile_options(performance PUBLIC -std=c++17)
  target_include_directories(performance PUBLIC "${google_benchmark_path}/include" "${PROJECT_SOURCE_DIR}/include")
  target_link_libraries(performance benchmark pthread)
endif(BUILD_PERFORMANCE)
