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
for(auto [pos, vel] : zip::make_zip(pos, vel)) {
  // ...
  for(auto [c, v] : zip::make_zip(pos, vel)) {
    c += v * dt;
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

#### Todo:
* Support taking iterator pairs {begin, end} in the constructor
* Find out how to avoid implementing std::swap(std::tuple<_Elements...> &&, std::tuple<_Elements...> &&);
* Add checks for attempts to dereference past the end of one of the iterator limits
* More performance tests
