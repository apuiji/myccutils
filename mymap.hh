#pragma once

#include"myset.hh"
#include"xyz.hh"

namespace zlt::mymap {
  template<class K, class T>
  using Node = myset::Node<std::pair<K, T>>;

  template<class Comp>
  struct KeyComparator {
    const Comp &comp;
    KeyComparator(const Comp &comp) noexcept: comp(comp) {}
    template<class K, class T, class U>
    int operator ()(const std::pair<K, T> &kt, U &&u) noexcept {
      return comp(kt.first, std::move(u));
    }
  };

  template<class K, class T, class U, class Comp = Comparator>
  Node<K, T> *find(Node<K, T> *node, U &&u, const Comp &comp = {}) noexcept {
    return myset::find(node, std::forward<U>(u), KeyComparator(comp));
  }

  template<class K, class T, class U, class Comp = Comparator>
  static inline const Node<K, T> *find(const Node<K, T> *node, U &&u, const Comp &comp = {}) noexcept {
    return find(const_cast<Node<K, T> *>(node), std::forward<U>(u), comp);
  }

  /// @param[out] parent initialized by null, the parent node of found
  /// @return not null when already exists
  template<class K, class T, class U, class Comp = Comparator>
  Node<K, T> *&findToInsert(Node<K, T> *&parent, Node<K, T> *&node, U &&u, const Comp &comp = {}) noexcept {
    return myset::find(parent, node, std::forward<U>(u), KeyComparator(comp));
  }

  template<class K, class T, class U, class Comp, class Alloc>
  Node<K, T> *insert(Node<K, T> *&node, U &&u, const Comp &comp, Alloc &&alloc) {
    return myset::insert(node, std::forward<U>(u), KeyComparator(comp), std::forward<Alloc>(alloc));
  }

  template<class K, class T, class U, class Alloc>
  Node<K, T> *insert(Node<K, T> *&node, U &&u, Alloc &&alloc) {
    return insert(node, std::forward<U>(u), Comparator(), std::forward<Alloc>(alloc));
  }
}
