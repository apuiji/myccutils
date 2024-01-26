#pragma once

#include"xyz.hh"

namespace zlt::rbtree {
  struct Node {
    Node *parent;
    Node *children[0];
    Node *lchd;
    Node *rchd;
    bool red = true;
  };

  /// @param node not null
  Node *mostLeftNN(Node *node) noexcept;
  /// @param node not null
  Node *mostRightNN(Node *node) noexcept;
  /// @param node not null
  Node *mostTopNN(Node *node) noexcept;

  template<bool Right>
  static inline Node *mostSideNN(Node *node) noexcept {
    if constexpr (Right) {
      return mostRightNN(node);
    } else {
      return mostLeftNN(node);
    }
  }

  static inline Node *mostSideNN(Node *node, bool right) noexcept {
    return (right ? mostRightNN : mostLeftNN)(node);
  }

  static inline Node *mostLeft(Node *node) noexcept {
    return node ? mostLeftNN(node) : nullptr;
  }

  static inline Node *mostRight(Node *node) noexcept {
    return node ? mostRightNN(node) : nullptr;
  }

  template<bool Right>
  static inline Node *mostSide(Node *node) noexcept {
    return node ? mostSideNN<Right>(node) : nullptr;
  }

  static inline Node *mostSide(Node *node, bool right) noexcept {
    return node ? mostSideNN(node, right) : nullptr;
  }

  static inline Node *mostTop(Node *node) noexcept {
    return node ? mostTopNN(node) : nullptr;
  }

  /// @param node not null
  Node *leftRotate(Node *node) noexcept;
  /// @param node not null
  Node *rightRotate(Node *node) noexcept;

  /// @param node not null
  template<bool Right>
  static inline Node *rotate(Node *node) noexcept {
    if constexpr (Right) {
      return rightRotate(node);
    } else {
      return leftRotate(node);
    }
  }

  /// @param node not null
  static inline Node *rotate(Node *node, bool right) noexcept {
    return (right ? rightRotate : leftRotate)(node);
  }

  /// @param node not null
  Node *nextLeft(Node *node) noexcept;
  /// @param node not null
  Node *nextRight(Node *node) noexcept;

  /// @param node not null
  template<bool Right>
  static inline Node *next(Node *node) noexcept {
    if constexpr (Right) {
      return nextRight(node);
    } else {
      return nextLeft(node);
    }
  }

  /// @param node not null
  static inline Node *next(Node *node, bool right) noexcept {
    return (right ? nextRight : nextLeft)(node);
  }

  /// LNR/RNL traversal
  template<std::derived_from<Node> T, bool Right = true>
  struct Iterator {
    T *node;
    Iterator(T *node = nullptr) noexcept: node(node) {}
    bool operator ==(Iterator<T> it) const noexcept {
      return node == it.node;
    }
    bool operator !=(Iterator<T> it) const noexcept {
      return node != it.node;
    }
    T &operator *() noexcept {
      return *node;
    }
    T *operator ->() noexcept {
      return node;
    }
    Iterator<T> &operator ++() noexcept {
      node = static_cast<T *>(next<Right>(node));
      return *this;
    }
    Iterator<T> operator ++(int) noexcept {
      auto next = static_cast<T *>(next<Right>(node));
      return std::exchange(node, next);
    }
  };

  int afterInsert(Node *&root, Node *node) noexcept;
  int beforeErase(Node *&root, Node *node) noexcept;
}
