#pragma once

#include"xyz.hh"

namespace zlt {
  struct BiTree {
    BiTree *parent;
    BiTree *children[0];
    BiTree *lchd;
    BiTree *rchd;
    BiTree(const BiTree *parent = nullptr) noexcept: parent((BiTree *) parent) {}
    void swap(BiTree &a) noexcept;
  };
}

namespace zlt::bitree {
  template<class Dtor>
  void clean(BiTree *tree, const Dtor &dtor) noexcept {
    if (!tree) [[unlikely]] {
      return;
    }
    auto lchd = tree->lchd;
    auto rchd = tree->rchd;
    dtor(tree);
    clean(lchd, dtor);
    clean(rchd, dtor);
  }

  /// @param tree requires not null
  /// @param side -1 top, 0 left, 1 right
  BiTree *mostSide(const BiTree *tree, int side) noexcept;

  // iterators begin
  /// @param tree requires not null
  /// @param xy 0 NLR, 1 NRL
  BiTree *nxy(const BiTree *tree, int xy) noexcept;

  /// @param tree requires not null
  /// @param xy 0 LNR, 1 RNL
  BiTree *xny(const BiTree *tree, int xy) noexcept;

  /// @param tree requires not null
  /// @param xy 0 LRN, 1 RLN
  BiTree *xyn(const BiTree *tree, int xy) noexcept;
  // iterators end

  /// @param tree requires not null and has child on side rightabout rotation
  BiTree *rotate(const BiTree *tree, bool right) noexcept;

  template<class Cmp, class T>
  BiTree *find(const BiTree *tree, const Cmp &cmp, T &&t) noexcept {
    if (!tree) [[unlikely]] {
      return nullptr;
    }
    int diff = cmp(std::forward<T>(t), *tree);
    if (!diff) {
      return (BiTree *) tree;
    }
    return find(tree->children[diff > 0], cmp, std::forward<T>(t));
  }

  template<class Cmp, class T>
  BiTree *&findForInsert(BiTree *&parent, BiTree *&tree, const Cmp &cmp, T &&t) noexcept {
    if (!tree) [[unlikely]] {
      return tree;
    }
    int diff = cmp(std::forward<T>(t), *tree);
    if (!diff) {
      return tree;
    }
    parent = tree;
    return findForInsert(parent, tree->children[diff > 0], cmp, std::forward<T>(t));
  }
}
