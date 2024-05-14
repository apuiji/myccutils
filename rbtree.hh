#pragma once

#include"bitree.hh"

namespace zlt {
  struct RBTree: BiTree {
    bool red;
    RBTree(const RBTree *parent = nullptr) noexcept: BiTree(parent), red(true) {}
    void swap(RBTree &a) noexcept;
  };
}

namespace zlt::rbtree {
  void afterInsert(RBTree *&root, RBTree *tree) noexcept;
  void beforeErase(RBTree *&root, RBTree *tree) noexcept;
}
