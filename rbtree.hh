#pragma once

#include<concepts>

namespace zlt::rbtree {
  struct Node {
    Node *parent;
    Node *children[0];
    Node *lchild;
    Node *rchild;
    bool red = true;
  };

  /// @param node not null
  Node *mostLeft(Node *node) noexcept;
  /// @param node not null
  Node *mostRight(Node *node) noexcept;
  /// @param node not null
  Node *mostTop(Node *node) noexcept;

  /// @param node not null
  template<bool Right>
  static inline Node *mostSide(Node *node) noexcept {
    if constexpr (Right) {
      return mostRight(node);
    } else {
      return mostLeft(node);
    }
  }

  /// @param node not null
  Node *nextLeft(const Node *node) noexcept;
  /// @param node not null
  Node *nextRight(const Node *node) noexcept;

  /// @param node not null
  template<bool Right>
  static inline Node *next(const Node *node) noexcept {
    if constexpr (Right) {
      return nextRight(node);
    } else {
      return nextLeft(node);
    }
  }

  /// @param node not null, right child not null also
  /// @return new axis node
  Node *leftRotate(Node *node) noexcept;
  /// @param node not null, left child not null also
  /// @return new axis node
  Node *rightRotate(Node *node) noexcept;

  /// @param node not null, other side child not null also
  /// @return new axis node
  template<bool Right>
  static inline Node *rotate(Node *node) noexcept {
    if constexpr (Right) {
      return rightRotate(node);
    } else {
      return leftRotate(node);
    }
  }

  int afterInsert(Node *&root, Node *node) noexcept;
  int beforeErase(Node *&root, Node *node) noexcept;

  template<std::derived_from<Node> T, bool Right = true>
  struct Iterator {
    Node *node;
    Iterator(Node *node = nullptr) noexcept: node(node) {}
    bool operator ==()(Iterator<T, Right> it) const noexcept {
      return node == it.node;
    }
    bool operator !=()(Iterator<T, Right> it) const noexcept {
      return node != it.node;
    }
    T &operator *() const noexcept {
      return static_cast<T &>(*node);
    }
    T *operator ->() const noexcept {
      return static_cast<T *>(node);
    }
    Iterator<T, Right> &operator ++() noexcept {
      node = next<Right>(node);
      return *this;
    }
    Iterator<T, Right> operator ++(int) noexcept {
      return std::exchange(node, next<Right>(node));
    }
  };
}
