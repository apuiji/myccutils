#pragma once

#include<concepts>

namespace zlt::myiter {
  template<class It>
  using IterateTo = decltype(*std::declval<It>());

  template<class It, class T>
  concept IteratorOf = std::is_convertible_v<IterateTo<It>, T>;

  template<class It>
  static inline bool operator ==(const It &a, const It &b) noexcept {
    return a.value == b.value;
  }

  template<class It>
  static inline bool operator !=(const It &a, const It &b) noexcept {
    return a.value != b.value;
  }

  template<class It>
  static inline It &operator ++(It &it) noexcept {
    ++it.value;
    return it;
  }

  template<class It>
  static inline It &operator ++(It &it, int) noexcept {
    It it1 = it;
    ++it.value;
    return it1;
  }

  template<class T, class U>
  concept RangeOf = requires (T &t) {
    { t.begin() } -> IteratorOf<U>;
    { t.end() } -> IteratorOf<U>;
  };

  template<class It, class Transform>
  struct TransformIterator {
    It value;
    const Transform &transform;
    TransformIterator(const It &value, const Transform &transform) noexcept: value(value), transform(transform) {}
    decltype(std::declval<Transform>()(std::declval<IterateTo<It>>())) operator *() noexcept {
      return transform(*value);
    }
  };

  template<class It, class Transform>
  struct TransformRange {
    It beginv;
    It endv;
    Transform transform;
    TransformRange(const It &beginv, const It &endv, Transform &&transform) noexcept:
    beginv(beginv), endv(endv), transform(std::move(transform)) {}
    auto begin() const noexcept {
      return TransformIterator<It, Transform>(beginv, transform);
    }
    auto end() const noexcept {
      return TransformIterator<It, Transform>(endv, transform);
    }
  };

  template<class It, class Transform>
  static inline auto makeTransformRange(const It &begin, const It &end, Transform &&transform) noexcept {
    return TransformRange<It, Transform>(begin, end, std::move(transform));
  }

  template<class It>
  static inline auto makePointerToRange(const It &begin, const It &end) noexcept {
    return makeTransformRange(begin, end, [] (IterateTo<It> value) { return *value; });
  }
}
