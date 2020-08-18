
#include <random>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "zip.hpp"

TEST_CASE("get, difference, compare, increment, set",
          "[Zip]") {
  using ZipT = zip::Zip<std::random_access_iterator_tag,
                        std::vector<std::pair<int, int>>,
                        std::vector<int>>;
  typename std::iterator_traits<ZipT::iterator>::value_type
      t{{234, 65}, 5};
  std::vector<std::pair<int, int>> vp{{0, 5}};
  std::vector<int> vi{6};
  ZipT z(vp, vi);
  ZipT z2 = z;
  SECTION("const iterator") {
    ZipT::const_iterator ci_1(z.cbegin());
    ZipT::const_iterator ci_2 = z2.cbegin();
    ZipT::const_iterator ci_3 = z2.cend();
    REQUIRE(ci_1 == ci_2);
    REQUIRE(!(ci_1 < ci_2));
    REQUIRE(ci_1 <= ci_2);
    REQUIRE(!(ci_1 > ci_2));
    REQUIRE(ci_1 >= ci_2);

    REQUIRE(!(ci_1 == ci_3));
    REQUIRE(ci_1 < ci_3);
    REQUIRE(ci_1 <= ci_3);
    REQUIRE(!(ci_1 > ci_3));
    REQUIRE(!(ci_1 >= ci_3));

    REQUIRE(ci_2 - ci_1 == 0);
    REQUIRE(ci_3 - ci_1 == 1);

    std::tuple<const std::pair<int, int> &, const int &>
        const_deref(*ci_2);
    REQUIRE(std::get<0>(const_deref) == vp[0]);
    REQUIRE(std::get<1>(const_deref) == vi[0]);

    ci_2++;
    REQUIRE(ci_2 - ci_1 == 1);
    REQUIRE(ci_2 == ci_3);
    ci_2--;
    REQUIRE(ci_3 - ci_2 == 1);
    REQUIRE(ci_2 == ci_1);
  }
  SECTION("iterator") {
    ZipT::iterator iter_1(z.begin());
    ZipT::iterator iter_2 = z2.begin();
    ZipT::iterator iter_3 = z2.end();
    REQUIRE(iter_1 == iter_2);
    REQUIRE(!(iter_1 < iter_2));
    REQUIRE(iter_1 <= iter_2);
    REQUIRE(!(iter_1 > iter_2));
    REQUIRE(iter_1 >= iter_2);

    REQUIRE(!(iter_1 == iter_3));
    REQUIRE(iter_1 < iter_3);
    REQUIRE(iter_1 <= iter_3);
    REQUIRE(!(iter_1 > iter_3));
    REQUIRE(!(iter_1 >= iter_3));

    REQUIRE(iter_2 - iter_1 == 0);
    REQUIRE(iter_3 - iter_1 == 1);

    std::tuple<const std::pair<int, int> &, const int &>
        deref(*iter_2);
    REQUIRE(std::get<0>(deref) == vp[0]);
    REQUIRE(std::get<1>(deref) == vi[0]);

    iter_2++;
    REQUIRE(iter_2 - iter_1 == 1);
    REQUIRE(iter_2 == iter_3);
    iter_2--;
    REQUIRE(iter_3 - iter_2 == 1);
    REQUIRE(iter_2 == iter_1);

    *iter_2 =
        std::tuple<std::pair<int, int>, int>{{43, 65}, 12};
    REQUIRE(std::get<1>(*iter_2) == 12);
    *iter_2 =
        std::tuple<std::pair<int, int>, int>{{43, 65}, 17};
    REQUIRE(std::get<1>(*iter_2) == 17);
  }
}

TEST_CASE("for auto loop", "[Zip]") {
  using Array = std::array<int, 120>;
  Array arr_1, arr_2, arr_3;
  std::random_device rd;
  std::mt19937_64 rng(rd());
  std::uniform_int_distribution<> pdf(
      std::numeric_limits<int>::min() / 2,
      std::numeric_limits<int>::max() / 2);
  for(int &i : arr_1) {
    i = pdf(rng);
  }
  for(int &i : arr_2) {
    i = pdf(rng);
  }
  for(auto [a1, a2, a3] :
      zip::make_zip(arr_1, arr_2, arr_3)) {
    a3 = a1 + a2;
  }
  for(auto [a1, a2, a3] :
      zip::make_zip(arr_1, arr_2, arr_3)) {
    REQUIRE(a3 == a1 + a2);
  }
}

TEST_CASE("sort", "[Zip]") {
  using Array = std::array<int, 4096>;
  Array arr_1, arr_2, arr_3;
  std::random_device rd;
  std::mt19937_64 rng(rd());
  std::uniform_int_distribution<> pdf(
      std::numeric_limits<int>::min() / 2,
      std::numeric_limits<int>::max() / 2);
  auto z = zip::make_zip(arr_1, arr_2, arr_3);
  for(auto [i1, i2, i3] : z) {
    i1 = pdf(rng);
    i2 = pdf(rng);
    i3 = pdf(rng);
  }
  std::sort(z.begin(), z.end(),
            [](const decltype(z)::value_type &v1,
               const decltype(z)::value_type &v2) {
              return std::get<0>(v1) < std::get<0>(v2);
            });
  int i1_min = std::numeric_limits<int>::min();
  for(auto [i1, i2, i3] :
      zip::make_zip(arr_1, arr_2, arr_3)) {
    REQUIRE(i1_min <= i1);
    i1_min = i1;
  }
}
