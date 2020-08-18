
#ifndef _ZIP_INTERNAL_HPP_
#define _ZIP_INTERNAL_HPP_

#include <tuple>
#include <utility>

namespace zip_internal_ {

// Converters for the standard iterator types
struct value_converter {
  template <typename container>
  using convert = typename container::value_type;
};

struct reference_converter {
  template <typename container>
  using convert = typename container::reference;
};

struct const_reference_converter {
  template <typename container>
  using convert = typename container::const_reference;
};

struct pointer_converter {
  template <typename container>
  using convert = typename container::pointer;
};

struct const_pointer_converter {
  template <typename container>
  using convert = typename container::const_pointer;
};

struct iterator_converter {
  template <typename container>
  using convert = typename container::iterator;
};

struct begin_iterator_converter
    : public iterator_converter {
  template <typename container>
  convert<container> operator()(container &c) const {
    return c.begin();
  }
};

struct end_iterator_converter : public iterator_converter {
  template <typename container>
  convert<container> operator()(container &c) const {
    return c.end();
  }
};

struct const_iterator_converter {
  template <typename container>
  using convert = typename container::const_iterator;
};

// Conversions to apply to tuple elements
struct const_begin_iterator_converter
    : public const_iterator_converter {
  template <typename container>
  convert<container> operator()(const container &c) const {
    return c.cbegin();
  }
};

struct const_end_iterator_converter
    : public const_iterator_converter {
  template <typename container>
  convert<container> operator()(const container &c) const {
    return c.cend();
  }
};

struct const_iterator_deref {
  template <typename iter_t>
  const typename std::iterator_traits<iter_t>::reference
  operator()(const iter_t &i) const noexcept {
    return *i;
  }
};

struct iterator_deref {
  template <typename iter_t>
  typename std::iterator_traits<iter_t>::reference
  operator()(iter_t &i) const noexcept {
    return *i;
  }
};

struct iterator_add {
  int summand;
  explicit iterator_add(const int s) : summand(s) {}

  template <typename iter_t>
  iter_t operator()(iter_t &i) {
    i += summand;
    return i;
  }
};

struct iterator_incr {
  template <typename iter_t>
  iter_t operator()(iter_t &i) {
    ++i;
    return i;
  }
};

struct iterator_decr {
  template <typename iter_t>
  iter_t operator()(iter_t &i) {
    --i;
    return i;
  }
};

// Source for the tuple_map object:
// https://codereview.stackexchange.com/questions/193420/apply-a-function-to-each-element-of-a-tuple-map-a-tuple
template <class F, typename Tuple, size_t... Is>
auto tuple_map_impl(Tuple t, F f,
                    std::index_sequence<Is...>) {
  return std::make_tuple(f(std::get<Is>(t))...);
}

template <class F, typename... Args>
auto tuple_map(const std::tuple<Args...> &t, F f) {
  return tuple_map_impl(
      t, f, std::make_index_sequence<sizeof...(Args)>{});
}

template <class F, typename Tuple, size_t... Is>
auto ref_tuple_map_impl(Tuple &t, F f,
                        std::index_sequence<Is...>) {
  return std::forward_as_tuple(f(std::get<Is>(t))...);
}

template <class F, typename... Args>
auto ref_tuple_map(std::tuple<Args...> &t, F f) {
  return ref_tuple_map_impl(
      t, f, std::make_index_sequence<sizeof...(Args)>{});
}

template <class F, typename Tuple, size_t... Is>
auto ref_tuple_map_impl(const Tuple &t, F f,
                        std::index_sequence<Is...>) {
  return std::forward_as_tuple(f(std::get<Is>(t))...);
}

template <class F, typename... Args>
auto ref_tuple_map(const std::tuple<Args...> &t, F f) {
  return ref_tuple_map_impl(
      t, f, std::make_index_sequence<sizeof...(Args)>{});
}

}  // namespace zip_internal_

namespace std {

/// swap
// Terrible hack which is probably a bad idea - copy the std
// implementation of lvalue tuple swaps, change it for
// rvalues
template <typename... elements>
constexpr typename enable_if<
    conjunction<is_swappable<elements>...>::value>::type
swap(tuple<elements...> &&x,
     tuple<elements...> &&y) noexcept(noexcept(x.swap(y))) {
  x.swap(y);
}

template <typename... _Elements>
_GLIBCXX20_CONSTEXPR typename enable_if<
    !__and_<__is_swappable<_Elements>...>::value>::type
swap(tuple<_Elements...> &&,
     tuple<_Elements...> &&) = delete;

}  // namespace std

#endif  // _ZIP_INTERNAL_HPP_
