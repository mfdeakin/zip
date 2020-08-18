
#include <benchmark/benchmark.h>

#include <memory>
#include <typeinfo>

#include "zip.hpp"

template <size_t num_elem>
static void BM_1D_Index_Initialize(
    benchmark::State &state) {
  std::array<double, num_elem> pos;
  std::array<double, num_elem> vel;
  while(state.KeepRunning()) {
    for(int i1 = 0; i1 < num_elem; i1++) {
      double *p = &pos[i1];
      benchmark::DoNotOptimize(*p = 0);
      double *v = &vel[i1];
      benchmark::DoNotOptimize(*v = 0);
    }
  }
}

template <size_t num_elem>
static void BM_1D_Zip_Initialize(benchmark::State &state) {
  std::array<double, num_elem> pos;
  std::array<double, num_elem> vel;
  while(state.KeepRunning()) {
    for(auto [p, v] : zip::make_zip(pos, vel)) {
      benchmark::DoNotOptimize(p = 0);
      benchmark::DoNotOptimize(v = 0);
    }
  }
}

template <size_t num_elem>
static void BM_1D_Index_Iterate(benchmark::State &state) {
  std::array<double, num_elem> pos;
  std::array<double, num_elem> vel;
  while(state.KeepRunning()) {
    for(int i1 = 0; i1 < num_elem; i1++) {
      double *p = &pos[i1];
      benchmark::DoNotOptimize(*p);
      double *v = &vel[i1];
      benchmark::DoNotOptimize(*v);
    }
  }
}

template <size_t num_elem>
static void BM_1D_Zip_Iterate(benchmark::State &state) {
  std::array<double, num_elem> pos;
  std::array<double, num_elem> vel;
  while(state.KeepRunning()) {
    for(auto [p, v] : zip::make_zip(pos, vel)) {
      benchmark::DoNotOptimize(p);
      benchmark::DoNotOptimize(v);
    }
  }
}

template <size_t num_elem, size_t num_dim>
static void BM_2D_Index_Initialize(
    benchmark::State &state) {
  std::array<std::array<double, num_dim>, num_elem> pos;
  std::array<std::array<double, num_dim>, num_elem> vel;
  while(state.KeepRunning()) {
    for(int i1 = 0; i1 < num_elem; i1++) {
      for(int i2 = 0; i2 < num_dim; i2++) {
        double *p = &pos[i1][i2];
        benchmark::DoNotOptimize(*p = 0);
        double *v = &vel[i1][i2];
        benchmark::DoNotOptimize(*v = 0);
      }
    }
  }
}

template <size_t num_elem, size_t num_dim>
static void BM_2D_Zip_Initialize(benchmark::State &state) {
  std::array<std::array<double, num_dim>, num_elem> pos;
  std::array<std::array<double, num_dim>, num_elem> vel;
  while(state.KeepRunning()) {
    for(auto [p, v] : zip::make_zip(pos, vel)) {
      for(auto [p_x, v_x] : zip::make_zip(p, v)) {
        benchmark::DoNotOptimize(p_x = 0);
        benchmark::DoNotOptimize(v_x = 0);
      }
    }
  }
}

template <size_t num_elem, size_t num_dim>
static void BM_2D_Index_Iterate(benchmark::State &state) {
  std::array<std::array<double, num_dim>, num_elem> pos;
  std::array<std::array<double, num_dim>, num_elem> vel;
  while(state.KeepRunning()) {
    for(int i1 = 0; i1 < num_elem; i1++) {
      for(int i2 = 0; i2 < num_dim; i2++) {
        const double *p = &pos[i1][i2];
        benchmark::DoNotOptimize(*p);
        const double *v = &vel[i1][i2];
        benchmark::DoNotOptimize(*v);
      }
    }
  }
}

template <size_t num_elem, size_t num_dim>
static void BM_2D_Zip_Iterate(benchmark::State &state) {
  std::array<std::array<double, num_dim>, num_elem> pos;
  std::array<std::array<double, num_dim>, num_elem> vel;
  while(state.KeepRunning()) {
    for(auto [p, v] : zip::make_zip(pos, vel)) {
      for(auto [p_x, v_x] : zip::make_zip(p, v)) {
        benchmark::DoNotOptimize(p_x);
        benchmark::DoNotOptimize(v_x);
      }
    }
  }
}

int main(int argc, char **argv) {
  constexpr size_t dim = 3;
  constexpr size_t small_num_elems = 1 << 5;
  constexpr size_t large_num_elems = 1 << 16;
  benchmark::RegisterBenchmark(
          "BM_1D_Small_Index_Iterate",
          BM_1D_Index_Iterate<small_num_elems>);
  benchmark::RegisterBenchmark(
          "BM_1D_Small_Zip_Iterate",
          BM_1D_Zip_Iterate<small_num_elems>);
  benchmark::RegisterBenchmark(
          "BM_1D_Small_Index_Initialize",
          BM_1D_Index_Initialize<small_num_elems>);
  benchmark::RegisterBenchmark(
          "BM_1D_Small_Zip_Initialize",
          BM_1D_Zip_Initialize<small_num_elems>);

  benchmark::RegisterBenchmark(
          "BM_2D_Small_Index_Iterate",
          BM_2D_Index_Iterate<small_num_elems, dim>);
  benchmark::RegisterBenchmark(
          "BM_2D_Small_Zip_Iterate",
          BM_2D_Zip_Iterate<small_num_elems, 3>);
  benchmark::RegisterBenchmark(
          "BM_2D_Small_Index_Initialize",
          BM_2D_Index_Initialize<small_num_elems, dim>);
  benchmark::RegisterBenchmark(
          "BM_2D_Small_Zip_Initialize",
          BM_2D_Zip_Initialize<small_num_elems, dim>);

  benchmark::RegisterBenchmark(
          "BM_1D_Large_Index_Iterate",
          BM_1D_Index_Iterate<large_num_elems>);
  benchmark::RegisterBenchmark(
          "BM_1D_Large_Zip_Iterate",
          BM_1D_Zip_Iterate<large_num_elems>);
  benchmark::RegisterBenchmark(
          "BM_1D_Large_Index_Initialize",
          BM_1D_Index_Initialize<large_num_elems>);
  benchmark::RegisterBenchmark(
          "BM_1D_Large_Zip_Initialize",
          BM_1D_Zip_Initialize<large_num_elems>);

  benchmark::RegisterBenchmark(
          "BM_2D_Large_Index_Iterate",
          BM_2D_Index_Iterate<large_num_elems, dim>);
  benchmark::RegisterBenchmark(
          "BM_2D_Large_Zip_Iterate",
          BM_2D_Zip_Iterate<large_num_elems, 3>);
  benchmark::RegisterBenchmark(
          "BM_2D_Large_Index_Initialize",
          BM_2D_Index_Initialize<large_num_elems, dim>);
  benchmark::RegisterBenchmark(
          "BM_2D_Large_Zip_Initialize",
          BM_2D_Zip_Initialize<large_num_elems, dim>);

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}
