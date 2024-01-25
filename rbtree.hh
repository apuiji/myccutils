#pragma once

#include"xyz.hh"

namespace zlt {
  struct RBTreeNode {
    RBTreeNode *parent;
    RBTreeNode *lchd;
    RBTreeNode *rchd;
    bool red = true;
  };

  RBTreeNode *mostLeftNN(RBTreeNode *node) noexcept;
  RBTreeNode *mostRightNN(RBTreeNode *node) noexcept;
  RBTreeNode *mostTopNN(RBTreeNode *node) noexcept;

  static inline RBTreeNode *mostLeft(RBTreeNode *node) noexcept {
    return node ? mostLeftNN(node) : nullptr;
  }

  static inline RBTreeNode *mostRight(RBTreeNode *node) noexcept {
    return node ? mostRightNN(node) : nullptr;
  }

  static inline RBTreeNode *mostTop(RBTreeNode *node) noexcept {
    return node ? mostTopNN(node) : nullptr;
  }

  RBTreeNode *leftRotate(RBTreeNode *node) noexcept;
  RBTreeNode *rightRotate(RBTreeNode *node) noexcept;

  /// lrn
  struct RBTreeIterator {
    RBTreeNode *node;
    RBTreeIterator(RBTreeNode *node = nullptr) noexcept: node(node) {}
    bool operator ==(RBTreeIterator it) const noexcept {
      return node == it.node;
    }
    bool operator !=(RBTreeIterator it) const noexcept {
      return node != it.node;
    }
    RBTreeNode &operator *() noexcept {
      return *node;
    }
    RBTreeNode *operator ->() noexcept {
      return node;
    }
  };

  RBTreeIterator next(const RBTreeIterator &it) noexcept;

  static inline RBTreeIterator &operator ++(RBTreeIterator &it) noexcept {
    return it = next(it);
  }

  static inline RBTreeIterator operator ++(RBTreeIterator &it, int) noexcept {
    return std::exchange(it, next(it));
  }

  int afterInsert(RBTreeNode *node) noexcept;
  int beforeErase(RBTreeNode *node) noexcept;
}
