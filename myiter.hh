#pragma once

#include<algorithm>
#include<concepts>
#include<iterator>

namespace zlt::myiter {
  template<class It, class T>
  concept IteratorOf = std::is_same_v<std::iter_value_t<It>, T>;

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
  struct SelfIterator: Iterator {
    using value_type = It;
    It value;
    SelfIterator(const It &value): value(value) {}
    value_type operator *() const {
      return value;
    }
  };

  template<class It>
  struct SelfRange {
    It beginv;
    It endv;
    SelfRange(const It &beginv, const It &endv): beginv(beginv), endv(endv) {}
    auto begin() const {
      return SelfIterator<It>(beginv);
    }
    auto end() const {
      return SelfIterator<It>(endv);
    }
  };

  template<class It>
  static inline auto makeSelfRange(const It &begin, const It &end) {
    return SelfRange<It>(begin, end);
  }

  template<class T>
  static inline auto makeSelfRange(const T &t) {
    return makeSelfRange(t.begin(), t.end());
  }

  template<class It, class Trans>
  struct TransformIterator: Iterator {
    using value_type = decltype(std::declval<Trans>()(std::declval<std::iter_value_t<It>>()));
    It value;
    const Trans &trans;
    TransformIterator(const It &value, const Trans &trans): value(value), trans(trans) {}
    value_type operator *() const {
      return trans(*value);
    }
  };

  template<class It, class Trans>
  struct TransformRange {
    It beginv;
    It endv;
    Trans trans;
    TransformRange(const It &beginv, const It &endv, Trans &&trans): beginv(beginv), endv(endv), trans(std::move(trans)) {}
    auto begin() const {
      return TransformIterator<It, Trans>(beginv, trans);
    }
    auto end() const {
      return TransformIterator<It, Trans>(endv, trans);
    }
  };

  template<class It, class Trans>
  static inline auto makeTransformRange(const It &begin, const It &end, Trans &&trans) {
    return TransformRange<It, Trans>(begin, end, std::move(trans));
  }

  template<class T, class Trans>
  static inline auto makeTransformRange(const T &t, Trans &&trans) {
    return makeTransformRange(t.begin(), t.end(), std::forward<Trans>(trans));
  }

  template<class T, class It>
  static inline auto makeCastRange(const It &begin, const It &end) {
    return makeTransformRange(begin, end, [] (std::iter_value_t<It> value) { return (T) value; });
  }

  template<class T, class U>
  static inline auto makeCastRange(const U &u) {
    return makeCastRange<T>(u.begin(), u.end());
  }

  template<size_t I, class It>
  static inline auto makeElementAtRange(const It &begin, const It &end) {
    return makeTransformRange(begin, end, [] (std::iter_value_t<It> value) { return std::get<I>(value); });
  }

  template<size_t I, class T>
  static inline auto makeElementAtRange(const T &t) {
    return makeElementAtRange<I>(t.begin(), t.end());
  }

  template<class It>
  static inline auto makePointerToRange(const It &begin, const It &end) {
    return makeTransformRange(begin, end, [] (std::iter_value_t<It> value) { return *value; });
  }

  template<class T>
  static inline auto makePointerToRange(const T &t) {
    return makePointerToRange(t.begin(), t.end());
  }

  template<class T, class U>
  static inline auto forEach(const T &t, U &&u) {
    return std::for_each(t.begin(), t.end(), std::forward<U>(u));
  }
}
