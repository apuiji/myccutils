#pragma once

#include"myset.hh"

namespace zlt::mymap {
  template<class K, class T>
  using Node = myset::Node<std::pair<K, T>>;

  template<class Comp>
  struct KeyCompare {
    const Comp &comp;
    KeyCompare(const Comp &comp) noexcept: comp(comp) {}
    template<class U, class K, class T>
    int operator ()(U &&u, const std::pair<K, T> &kt) const noexcept {
      return comp(std::forward<U>(u), kt.first);
    }
  };

  template<class K, class T, class U, class Comp = std::compare_three_way>
  Node<K, T> *find(Node<K, T> *node, U &&u, const Comp &comp = {}) noexcept {
    return myset::find(node, std::forward<U>(u), KeyCompare(comp));
  }

  template<class K, class T, class U, class Comp = std::compare_three_way>
  static inline const Node<K, T> *find(const Node<K, T> *node, U &&u, const Comp &comp = {}) noexcept {
    return find(const_cast<Node<K, T> *>(node), std::forward<U>(u), comp);
  }

  /// @param[out] parent initialized by null, the parent node of found
  /// @return not null when already exists
  template<class K, class T, class U, class Comp = std::compare_three_way>
  static inline auto findToInsert(Node<K, T> *&node, U &&u, const Comp &comp = {}) noexcept {
    return myset::findToInsert(node, std::forward<U>(u), KeyCompare(comp));
  }
}
