#pragma once

#include<concepts>

namespace zlt::myiter {
  template<class It>
  using IterateTo = decltype(*std::declval<It>());

  template<class It, class T>
  concept IteratorOf = std::is_convertible_v<IterateTo<It>, T>;

  struct Iterator {};

  template<std::derived_from<Iterator> It>
  static inline bool operator ==(const It &a, const It &b) noexcept {
    return a.value == b.value;
  }

  template<std::derived_from<Iterator> It>
  static inline bool operator !=(const It &a, const It &b) noexcept {
    return a.value != b.value;
  }

  template<std::derived_from<Iterator> It>
  static inline It &operator ++(It &it) noexcept {
    ++it.value;
    return it;
  }

  template<std::derived_from<Iterator> It>
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

  template<class It>
  struct CommonIterator: Iterator {
    It value;
    CommonIterator(const It &value) noexcept: value(value) {}
    IterateTo<It> operator *() {
      return *value;
    }
  };

  template<class It>
  struct CommonRange {
    It beginv;
    It endv;
    CommonRange(It &&beginv, It &&endv) noexcept: beginv(std::move(beginv)), endv(std::move(endv)) {}
    CommonIterator<It> begin() const {
      return beginv;
    }
    CommonIterator<It> end() const {
      return endv;
    }
  };

  template<class It>
  static inline auto makeCommonRange(It &&begin, It &&end) {
    return CommonRange<It>(std::move(begin), std::move(end));
  }

  template<class It, class Trans>
  struct TransformIterator: Iterator {
    It value;
    const Trans &trans;
    TransformIterator(const It &value, const Trans &trans) noexcept: value(value), trans(trans) {}
    decltype(std::declval<Trans>()(std::declval<IterateTo<It>>())) operator *() {
      return trans(*value);
    }
  };

  template<class It, class Trans>
  struct TransformRange {
    It beginv;
    It endv;
    Trans trans;
    TransformRange(It &&beginv, It &&endv, Trans &&trans) noexcept:
    beginv(std::move(beginv)), endv(std::move(endv)), trans(std::move(trans)) {}
    auto begin() const {
      return TransformIterator<It, Trans>(beginv, trans);
    }
    auto end() const {
      return TransformIterator<It, Trans>(endv, trans);
    }
  };

  template<class It, class Trans>
  static inline auto makeTransformRange(It &&begin, It &&end, Trans &&trans) noexcept {
    return TransformRange<It, Trans>(std::move(begin), std::move(end), std::move(trans));
  }

  template<size_t I, class It>
  static inline auto makeElementAtRange(It &&begin, It &&end) noexcept {
    return makeTransformRange(std::move(begin), std::move(end), [] (IterateTo<It> value) { return std::get<I>(value); });
  }

  template<class It>
  static inline auto makePointerToRange(It &&begin, It &&end) noexcept {
    return makeTransformRange(
      std::move(begin),
      std::move(end),
      [] (IterateTo<It> value) { return *value; });
  }
}
