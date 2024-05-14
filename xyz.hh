#pragma once

#include<compare>
#include<concepts>
#include<memory>

namespace zlt {
  template<class T, class U>
  static inline constexpr size_t offsetOf(T U::*m) noexcept {
    return (size_t) ((T *) nullptr)->*m;
  }

  template<class T>
  static inline constexpr T &containerOf(void *p, auto T::*m) noexcept {
    return *(T *) (p - offsetOf(m));
  }

  template<class T>
  static inline constexpr const T &containerOf(const void *p, auto T::*m) noexcept {
    return *(const T *) (p - offsetOf(m));
  }

  #ifdef __cpp_lib_three_way_comparison

  using Compare = std::compare_three_way;

  #else

  struct Compare {
    template<class T, class U>
    auto operator ()(T &&t, U &&u) const {
      return t <=> u;
    }
  };

  #endif

  template<class ...T>
  struct Dynamicastable {
    template<class U>
    requires (std::is_pointer_v<U>)
    bool operator ()(const U u) noexcept {
      return u && (dynamic_cast<const T*>(u) || ...);
    }
    template<class U>
    requires (!std::is_pointer_v<U>)
    bool operator ()(const U &u) noexcept {
      return operator ()(&u);
    }
  };

  template<class T, class ...U>
  static constexpr bool isAnyOf = (std::is_same_v<T, U> || ...);

  template<class T, class ...U>
  concept AnyOf = isAnyOf<T, U...>;

  template<std::invocable T>
  static inline auto makeGuard(T &&t) noexcept {
    struct Guard {
      T t;
      Guard(T &&t) noexcept: t(std::move(t)) {}
      ~Guard() {
        t();
      }
    };
    return Guard(std::move(t));
  }

  template<std::invocable T>
  static inline auto makeGuard(T &&t, bool &on) noexcept {
    struct Guard {
      T t;
      bool &on;
      Guard(T &&t, bool &on) noexcept: t(std::move(t)), on(on) {}
      ~Guard() {
        if (on) {
          t();
        }
      }
    };
    return Guard(std::move(t), on);
  }

  /// overloaded function resolve
  template<class ...Args>
  struct OFR {
    template<class R>
    constexpr auto operator ()(R (*f)(Args...)) const noexcept {
      return f;
    }
  };

  template<class T>
  static inline T remove(T &t) noexcept {
    return std::move(t);
  }
}
