#pragma once

#include<concepts>

namespace zlt {
  struct Compare {
    template<class T, class U>
    inline int operator ()(T &&t, U &&u) const noexcept {
      return t < u ? -1 : t > u ? 1 : 0;
    }
    template<class T, class U>
    inline bool operator ()(int &dest, T &&t, U &&u) const noexcept {
      if (t < u) {
        dest = -1;
        return true;
      }
      if (t > u) {
        dest = 1;
        return true;
      }
      if (t == u) {
        dest = 0;
        return true;
      }
      return false;
    }
  };

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

  template<class T, class U>
  static inline size_t offsetOf(T U::*memb) noexcept {
    U *p = nullptr;
    void *q = &(p->*memb);
    return (size_t) q;
  }

  template<class T, class U>
  static inline auto &containerOf(T &t, T U::*memb) noexcept {
    size_t off = offsetOf(memb);
    void *p = (char *) &t - off;
    return *(U *) p;
  }

  /// overloaded function resolve
  template<class R, class ...Args>
  static inline constexpr auto ofr(R (*f)(Args...)) noexcept {
    return f;
  }

  template<class T>
  static inline T remove(T &t) noexcept {
    return std::move(t);
  }

  template<class T>
  requires (!std::is_reference_v<T>)
  static inline constexpr auto rtol(T t = T()) noexcept {
    struct U {
      T t;
      U(T t) noexcept: t(t) {}
      operator T &() && noexcept {
        return t;
      }
      operator T *() && noexcept {
        return &t;
      }
    };
    return U(t);
  }

  template<class T>
  requires (std::is_rvalue_reference_v<T>)
  static inline auto rtol(T &&t) noexcept {
    struct U {
      T t;
      U(T &&t) noexcept: t(std::move(t)) {}
      operator T &() && noexcept {
        return t;
      }
      operator T *() && noexcept {
        return &t;
      }
    };
    return U(std::move(t));
  }
}
