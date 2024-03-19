#pragma once

#include<compare>
#include<functional>
#include"rbtree.hh"
#include"xyz.hh"

namespace zlt::myset {
  template<class T>
  struct Node: rbtree::Node {
    using Value = T;
    T value;
    Node() = default;
    Node(const T &value): value(value) {}
    Node(T &&value) noexcept: value(std::move(value)) {}
  };

  template<class T, class Del = std::default_delete<T>>
  int clean(Node<T> *node, const Del &del = {}) noexcept {
    if (!node) [[unlikely]] {
      return 0;
    }
    clean(static_cast<Node<T> *>(node->lchild), del);
    clean(static_cast<Node<T> *>(node->rchild), del);
    del(node);
    return 0;
  }

  template<class T, class U, class Comp = Compare>
  Node<T> *find(Node<T> *node, U &&u, const Comp &comp = {}) noexcept {
    if (!node) [[unlikely]] {
      return nullptr;
    }
    auto diff = comp(std::forward<U>(u), node->value);
    if (diff) {
      auto next = node->children[diff > 0];
      return find(static_cast<Node<T> *>(next), std::forward<U>(u), comp);
    } else {
      return node;
    }
  }

  template<class T, class U, class Comp = Compare>
  static inline const Node<T> *find(const Node<T> *node, U &&u, const Comp &comp = {}) noexcept {
    return find(const_cast<Node<T> *>(node), std::forward<U>(u), comp);
  }

  template<class T, class U, class Comp = Compare>
  Node<T> *&findToInsert1(Node<T> *&parent, Node<T> *&node, U &&u, const Comp &comp = {}) noexcept {
    if (!node) [[unlikely]] {
      return node;
    }
    int diff = comp(std::forward<U>(u), node->value);
    if (diff) {
      parent = node;
      auto &next = node->children[diff > 0];
      auto &next1 = reinterpret_cast<Node<T> *&>(next);
      return findToInsert1(parent, next1, std::forward<U>(u), comp);
    } else {
      return node;
    }
  }

  /// @return [slot, parent]
  template<class T, class U, class Comp = Compare>
  static inline auto findToInsert(Node<T> *&root, U &&u, const Comp &comp = {}) noexcept {
    Node<T> *parent = nullptr;
    auto &slot = findToInsert1(parent, root, std::forward<U>(u), comp);
    return std::pair<Node<T> **, Node<T> *>(&slot, parent);
  }
}
