#pragma once

#include<concepts>

namespace zlt {
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
      ~Guard() {
        t();
      }
    };
    return Guard(std::move(t));
  }

  /// overloaded function resolve
  template<class R, class ...Args>
  static inline constexpr auto ofr(R (*f)(Args...)) noexcept {
    return f;
  }

  template<class T>
  static inline T remove(T &&t) noexcept {
    return std::move(t);
  }

  template<class T>
  requires (!std::is_reference_v<T>)
  static inline constexpr auto rtol(T t) noexcept {
    struct U {
      T t;
      operator T &() && noexcept {
        return t;
      }
    };
    return U(t);
  }

  template<class T>
  requires (std::is_rvalue_reference_v<T>)
  static inline auto rtol(T &&t) noexcept {
    struct U {
      T t;
      operator T &() && noexcept {
        return t;
      }
    };
    return U(std::move(t));
  }
}
