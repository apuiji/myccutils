#pragma once

#include"myset.hh"
#include"xyz.hh"

namespace zlt::mymap {
  template<class K, class T>
  using Node = myset::Node<std::pair<K, T>>;

  template<class Comp>
  struct KeyCompare {
    const Comp &comp;
    KeyCompare(const Comp &comp) noexcept: comp(comp) {}
    template<class U, class K, class T>
    int operator ()(U &&u, const std::pair<K, T> &kt) const noexcept {
      return comp(std::move(u), kt.first);
    }
  };

  template<class K, class T, class U, class Comp = Compare>
  Node<K, T> *find(Node<K, T> *node, U &&u, const Comp &comp = {}) noexcept {
    return myset::find(node, std::forward<U>(u), KeyCompare(comp));
  }

  template<class K, class T, class U, class Comp = Compare>
  static inline const Node<K, T> *find(const Node<K, T> *node, U &&u, const Comp &comp = {}) noexcept {
    return find(const_cast<Node<K, T> *>(node), std::forward<U>(u), comp);
  }

  /// @param[out] parent initialized by null, the parent node of found
  /// @return not null when already exists
  template<class K, class T, class U, class Comp = Compare>
  Node<K, T> *&findToInsert(Node<K, T> *&parent, Node<K, T> *&node, U &&u, const Comp &comp = {}) noexcept {
    return myset::findToInsert(parent, node, std::forward<U>(u), KeyCompare(comp));
  }

  template<class K, class T, class U, class Comp, class Alloc>
  Node<K, T> *insert(Node<K, T> *&node, U &&u, const Comp &comp, Alloc &&alloc) {
    return myset::insert(node, std::forward<U>(u), KeyCompare(comp), std::forward<Alloc>(alloc));
  }

  template<class K, class T, class U, class Alloc>
  Node<K, T> *insert(Node<K, T> *&node, U &&u, Alloc &&alloc) {
    return insert(node, std::forward<U>(u), Compare(), std::forward<Alloc>(alloc));
  }
}
