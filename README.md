# Zip Iterator

A Zip iterator which enables iterating over multiple iterable containers of the same size.
Performance of the iterator was a major concern; tests indicate it's as good as manually iterating over all of the containers simultaneously with index iteration.
Note that the zip iterator defaults to assuming all of the iterable containers are of the Random Access type, this can be changed by specifying the type as the first template parameter.
This requires C++17, and should be compatible with CUDA code

Basic usage:

```c++
#include "zip.hpp"

std::vector<std::array<double, 3>> pos, vel;
// ...
for(auto [p, v] : zip::make_zip(pos, vel)) {
  // ...
  for(auto [p_x, v_x] : zip::make_zip(p, v)) {
    p_x += v_x * dt;
  }
}

// With the iterator tag specified
for(auto [pos, vel] : zip::make_zip<std::random_access_iterator_tag>(pos, vel)) {
  // ...
}

auto z = zip::make_zip(pos, vel);
std::sort(z.begin(), z.end(),
          [](const std::tuple<std::array<double, 3> &, std::array<double, 3> &> &lhs,
             const std::tuple<std::array<double, 3> &, std::array<double, 3> &> &rhs) {
            return std::get<0>(lhs)[0] < std::get<0>(rhs);
          });
```

#### Performance Results

* Processor: `Intel(R) Core(TM) i7-6700K CPU @ 4.00GHz, 8192 KB cache, 4 cores, 8 hyperthreads`
* Compiler: `g++ (GCC) 10.2.1 20200723 (Red Hat 10.2.1-1)`
* Compilation Flags: `-march=native -fstrict-aliasing -ffast-math -O3`
* Command line: `./performance --benchmark_repetitions=30 --benchmark_report_aggregates_only=true`
```
------------------------------------------------------------------------------
Benchmark                                    Time             CPU   Iterations
------------------------------------------------------------------------------
BM_Small_PosVel_Index_mean                 111 ns          111 ns           30
BM_Small_PosVel_Index_median               115 ns          115 ns           30
BM_Small_PosVel_Index_stddev              9.16 ns         9.10 ns           30

BM_Small_PosVel_Zip_mean                   103 ns          103 ns           30
BM_Small_PosVel_Zip_median                 101 ns          101 ns           30
BM_Small_PosVel_Zip_stddev                8.29 ns         8.27 ns           30

BM_Large_PosVel_Index_mean              237347 ns       236161 ns           30
BM_Large_PosVel_Index_median            238537 ns       237313 ns           30
BM_Large_PosVel_Index_stddev             16817 ns        16710 ns           30

BM_Large_PosVel_Zip_mean                239222 ns       238073 ns           30
BM_Large_PosVel_Zip_median              237924 ns       236708 ns           30
BM_Large_PosVel_Zip_stddev               19898 ns        19750 ns           30

BM_1D_Small_Index_Iterate_mean            22.4 ns         22.4 ns           30
BM_1D_Small_Index_Iterate_median          22.7 ns         22.6 ns           30
BM_1D_Small_Index_Iterate_stddev          3.73 ns         3.72 ns           30

BM_1D_Small_Zip_Iterate_mean              21.9 ns         21.8 ns           30
BM_1D_Small_Zip_Iterate_median            21.9 ns         21.8 ns           30
BM_1D_Small_Zip_Iterate_stddev            3.35 ns         3.34 ns           30

BM_1D_Small_Index_Initialize_mean         39.6 ns         39.5 ns           30
BM_1D_Small_Index_Initialize_median       40.5 ns         40.4 ns           30
BM_1D_Small_Index_Initialize_stddev       3.16 ns         3.15 ns           30

BM_1D_Small_Zip_Initialize_mean           44.1 ns         44.0 ns           30
BM_1D_Small_Zip_Initialize_median         45.1 ns         44.9 ns           30
BM_1D_Small_Zip_Initialize_stddev         4.43 ns         4.41 ns           30

* BM_2D_Small_Index_Iterate_mean          13.6 ns         13.6 ns           30
BM_2D_Small_Index_Iterate_median          13.6 ns         13.6 ns           30
BM_2D_Small_Index_Iterate_stddev          2.05 ns         2.04 ns           30

BM_2D_Small_Zip_Iterate_mean              59.8 ns         59.7 ns           30
BM_2D_Small_Zip_Iterate_median            61.3 ns         61.1 ns           30
BM_2D_Small_Zip_Iterate_stddev            6.23 ns         6.22 ns           30

BM_2D_Small_Index_Initialize_mean          119 ns          119 ns           30
BM_2D_Small_Index_Initialize_median        119 ns          119 ns           30
BM_2D_Small_Index_Initialize_stddev       10.1 ns         10.1 ns           30

BM_2D_Small_Zip_Initialize_mean            112 ns          111 ns           30
BM_2D_Small_Zip_Initialize_median          111 ns          111 ns           30
BM_2D_Small_Zip_Initialize_stddev         7.90 ns         7.89 ns           30

BM_1D_Large_Index_Iterate_mean          100420 ns        99985 ns           30
BM_1D_Large_Index_Iterate_median        100691 ns       100279 ns           30
BM_1D_Large_Index_Iterate_stddev          4935 ns         4905 ns           30

BM_1D_Large_Zip_Iterate_mean            103519 ns       103127 ns           30
BM_1D_Large_Zip_Iterate_median          103785 ns       103359 ns           30
BM_1D_Large_Zip_Iterate_stddev            2029 ns         2017 ns           30

BM_1D_Large_Index_Initialize_mean       147467 ns       147023 ns           30
BM_1D_Large_Index_Initialize_median     147093 ns       146600 ns           30
BM_1D_Large_Index_Initialize_stddev       6346 ns         6315 ns           30

BM_1D_Large_Zip_Initialize_mean         143974 ns       143518 ns           30
BM_1D_Large_Zip_Initialize_median       144939 ns       144486 ns           30
BM_1D_Large_Zip_Initialize_stddev         6778 ns         6750 ns           30

* BM_2D_Large_Index_Iterate_mean         27348 ns        27280 ns           30
BM_2D_Large_Index_Iterate_median         30076 ns        30004 ns           30
BM_2D_Large_Index_Iterate_stddev          5369 ns         5358 ns           30

BM_2D_Large_Zip_Iterate_mean            305916 ns       303949 ns           30
BM_2D_Large_Zip_Iterate_median          309785 ns       307707 ns           30
BM_2D_Large_Zip_Iterate_stddev           18784 ns        18568 ns           30

BM_2D_Large_Index_Initialize_mean       545104 ns       541892 ns           30
BM_2D_Large_Index_Initialize_median     546444 ns       543156 ns           30
BM_2D_Large_Index_Initialize_stddev      13655 ns        13447 ns           30

BM_2D_Large_Zip_Initialize_mean         552160 ns       549212 ns           30
BM_2D_Large_Zip_Initialize_median       553067 ns       550062 ns           30
BM_2D_Large_Zip_Initialize_stddev        12430 ns        12214 ns           30
```

\*The BM_2D_*_Index_Iterate benchmark is almost certainly wrong and needs to be fixed

#### Todo:
* Support taking iterator pairs {begin, end} in the constructor
* Find out how to avoid implementing std::swap(std::tuple<_Elements...> &&, std::tuple<_Elements...> &&);
* Add checks for attempts to dereference past the end of one of the iterator limits
* Understand and fix the BM_2D_Large_Index_Iterate issue
* Add more performance tests
