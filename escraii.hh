#pragma once

#include<memory>

namespace zlt {
  template<class T>
  struct EscRAII {
    char data[sizeof(T)];
    template<class ...Args>
    EscRAII(Args &&...args) {
      std::construct_at((T *) data, std::forward<Args>(args)...);
    }
    T *get() const noexcept {
      return (T *) data;
    }
    operator T &() const noexcept {
      return *get();
    }
    T *operator ->() const noexcept {
      return get();
    } 
  };
}
