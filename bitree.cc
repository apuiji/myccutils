#include"bitree.hh"

using namespace std;

namespace zlt {
  void BiTree::swap(BiTree &a) noexcept {
    std::swap(parent, a.parent);
    std::swap(lchd, a.lchd);
    std::swap(rchd, a.rchd);
  }
}

namespace zlt::bitree {

  BiTree *mostSide(const BiTree *tree, int side) noexcept {
    auto next = tree->children[side];
    return next ? mostSide(next, side) : (BiTree *) tree;
  }

  // iterators begin
  static BiTree *nxy1(const BiTree *tree, int xy) noexcept;

  BiTree *nxy(const BiTree *tree, int xy) noexcept {
    if (auto x = tree->children[xy]; x) {
      return x;
    }
    if (auto y = tree->children[!xy]; y) {
      return y;
    }
    return nxy1(tree, xy);
  }

  BiTree *nxy1(const BiTree *tree, int xy) noexcept {
    auto parent = tree->parent;
    if (!parent) {
      return nullptr;
    }
    if (tree == parent->children[xy] && parent->children[!xy]) {
      return parent->children[!xy];
    }
    return nxy1(parent, xy);
  }

  static BiTree *xny1(const BiTree *tree, int xy) noexcept;

  BiTree *xny(const BiTree *tree, int xy) noexcept {
    if (auto y = tree->children[!xy]; y) {
      return mostSide(y, xy);
    }
    return xny1(tree, xy);
  }

  BiTree *xny1(const BiTree *tree, int xy) noexcept {
    auto parent = tree->parent;
    if (!parent) {
      return nullptr;
    }
    if (tree == parent->children[xy]) {
      return parent;
    }
    return xny1(parent, xy);
  }

  BiTree *xyn(const BiTree *tree, int xy) noexcept {
    auto parent = tree->parent;
    if (!parent) {
      return nullptr;
    }
    if (tree == parent->children[xy] && parent->children[!xy]) {
      return mostSide(parent->children[!xy], xy);
    }
    return parent;
  }
  // iterators end

  BiTree *rotate(BiTree *tree, bool right) {
    auto parent = tree->parent;
    auto child = tree->children[!right];
    tree->parent = child;
    tree->children[!right] = child->children[right];
    child->parent = parent;
    child->children[right] = tree;
    if (parent) {
      parent->children[tree == parent->rchd] = child;
    }
    return child;
  }
}
