#pragma once

#include"rbtree.hh"
#include"xyz.hh"

namespace zlt::myset {
  template<class T>
  struct Node: rbtree::Node {
    T value;
    Node(const T &value): value(value) {}
    Node(T &&value) noexcept: value(std::move(value)) {}
  };

  template<class T, class Del = std::default_delete>
  int clean(Node<T> *node, const Del &del = {}) noexcept {
    if (!node) [[unlikely]] {
      return 0;
    }
    clean(static_cast<Node<T> *>(node->lchild), del);
    clean(static_cast<Node<T> *>(node->rchild), del);
    del(node);
    return 0;
  }

  template<class T, class U, class Comp = Comparator>
  Node<T> *find(Node<T> *node, U &&u, const Comp &comp = {}) noexcept {
    if (!node) [[unlikely]] {
      return nullptr;
    }
    int diff = comp(std::forward<U>(u), node->value);
    if (diff) {
      auto next = node->children[diff > 0];
      return find(static_cast<Node<T> *>(next), std::forward<U>(u), comp);
    } else {
      return node;
    }
  }

  template<class T, class U, class Comp = Comparator>
  static inline const Node<T> *find(const Node<T> *node, U &&u, const Comp &comp = {}) noexcept {
    return find(const_cast<Node<T> *>(node), std::forward<U>(u), comp);
  }

  /// @param[out] parent initialized by null, the parent node of found
  /// @return not null when already exists
  template<class T, class U, class Comp = Comparator>
  Node<T> *&findToInsert(Node<T> *&parent, Node<T> *&node, U &&u, const Comp &comp = {}) noexcept {
    if (!node) [[unlikely]] {
      return node;
    }
    int diff = comp(std::forward<U>(u), node->value);
    if (diff) {
      parent = node;
      auto &next = node->children[diff > 0];
      return findToInsert(parent, next, std::forward<U>(u), comp);
    } else {
      return node;
    }
  }

  template<class T, class U, class Comp, class Alloc>
  Node<T> *insert(Node<T> *&node, U &&u, const Comp &comp, Alloc &&alloc) {
    Node<T> *parent = nullptr;
    auto &a = findToInsert(parent, node, std::forward<U>(u), comp);
    if (!a) {
      a = alloc();
      a->parent = parent;
    }
    return a;
  }

  template<class T, class U, class Alloc>
  Node<T> *insert(Node<T> *&node, U &&u, Alloc &&alloc) {
    return insert(node, std::forward<U>(u), Comparator(), std::forward<Alloc>(alloc));
  }
}
