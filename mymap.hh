#pragma once

#include"rbtree.hh"

namespace zlt {
  template<class K, class T>
  struct MyMap {
    using Node = myset::Node<std::pair<K, T>>;
    using Key = K;
    using Value = T;
    void *root;
  };
}

namespace zlt::mymap {
  template<class K, class T>
  using Node = myset::Node<std::pair<K, T>>;

  template<class K, class T, class Cmp>
  struct KeyCompare {
    const Cmp &cmp;
    KeyCompare(const Cmp &cmp) noexcept: cmp(cmp) {}
    template<class U>
    auto operator ()(U &&u, const std::pair<K, T> &kt) const noexcept {
      return cmp(std::forward<U>(u), kt.first);
    }
  };

  template<class K, class T, class U, class Cmp = Compare>
  static inline void *find(const MyMap<K, T> &set, U &&u, const Cmp &cmp = {}) noexcept {
    return rbtree::find(set.root, KeyCompare<K, T>(cmp), std::forward<U>(u));
  }

  template<class K, class T, class U, class Cmp = Compare>
  static inline void *&findForInsert(void *&parent, MyMap<K, T> &set, U &&u, const Cmp &cmp = {}) noexcept {
    return *rbtree::findForInsert(parent, set.root, KeyCompare<K, T>(cmp), std::forward<U>(u));
  }
}
