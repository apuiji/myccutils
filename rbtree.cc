#include"rbtree.hh"

using namespace std;

namespace zlt {
  void RBTree::swap(RBTree &a) noexcept {
    BiTree::swap(a);
    std::swap(red, a.red);
  }
}

namespace zlt::rbtree {
  // after insert operations begin
  static void afterInsert1(RBTree *&root, RBTree *tree, RBTree *parent, RBTree *gparent) noexcept;
  static void afterInsert2(RBTree *&root, RBTree *tree, RBTree *parent, RBTree *gparent) noexcept;

  void afterInsert(RBTree *&root, RBTree *tree) noexcept {
    auto parent = static_cast<RBTree *>(tree->parent);
    if (!parent) [[unlikely]] {
      tree->red = false;
      root = tree;
      return;
    }
    if (!parent->red) {
      return;
    }
    auto gparent = static_cast<RBTree *>(parent->parent);
    auto uncle = static_cast<RBTree *>(gparent->children[parent == gparent->lchd]);
    if (uncle && uncle->red) {
      parent->red = false;
      uncle->red = false;
      gparent->red = true;
      afterInsert(root, gparent);
      return;
    }
    auto f = tree == parent->lchd && parent == gparent->lchd ? afterInsert1 : afterInsert2;
    f(root, tree, parent, gparent);
  }

  //     GB        PR        PB
  //    /  \      /  \      /  \_
  //   PR  UB -> TR  GB -> TR  GR
  //  /  \          /  \      /  \_
  // TR  SB        SB  UB    SB  UB
  void afterInsert1(RBTree *&root, RBTree *tree, RBTree *parent, RBTree *gparent) noexcept {
    bool left = tree == parent->lchd;
    bitree::rotate(gparent, !left);
    parent->red = false;
    gparent->red = true;
    if (root == gparent) {
      root = parent;
    }
  }

  //   GB        GB        __TR__        __TB__
  //  /  \      /  \      /      \      /      \_
  // PR  UB -> TR  UB -> PR      GB -> PR      GR
  //   \      /  \         \    /  \     \    /  \_
  //   TR    PR  YB        XB  YB  UB    XB  YB  UB
  //  /  \     \_
  // XB  YB    XB
  void afterInsert2(RBTree *&root, RBTree *tree, RBTree *parent, RBTree *gparent) noexcept {
    bool left = tree == parent->lchd;
    bitree::rotate(parent, !left);
    bitree::rotate(gparent, left);
    tree->red = false;
    gparent->red = true;
    if (root == gparent) {
      root = tree;
    }
  }
  // after insert operations end

  // before erase operations begin
  static void beforeErase1(RBTree *&root, RBTree *tree) noexcept;
  static void beforeErase2(RBTree *&root, RBTree *tree, RBTree *child) noexcept;
  static void beforeErase3(RBTree *&root, RBTree *tree) noexcept;

  void beforeErase(RBTree *&root, RBTree *tree) noexcept {
    beforeErase1(root, tree);
    if (tree->lchd) {
      beforeErase2(root, tree, static_cast<RBTree *>(tree->lchd));
      return;
    }
    if (tree->rchd) {
      beforeErase2(root, tree, static_cast<RBTree *>(tree->rchd));
      return;
    }
    if (tree->red) {
      return;
    }
    beforeErase3(root, tree);
  }

  void beforeErase1(RBTree *&root, RBTree *tree) noexcept {
    auto a = (RBTree *) bitree::xny(tree, 1);
    if (!a) {
      return;
    }
    tree->swap(*a);
    if (root == tree) {
      root = a;
    }
  }

  void beforeErase2(RBTree *&root, RBTree *tree, RBTree *child) noexcept {
    auto parent = tree->parent;
    child->parent = parent;
    child->red = false;
    if (parent) {
      parent->children[tree == parent->rchd] = child;
    } else {
      root = child;
    }
  }

  static void beforeErase4(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept;
  static void beforeErase5(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept;

  void beforeErase3(RBTree *&root, RBTree *tree) noexcept {
    auto parent = tree->parent;
    auto sibling = parent->children[tree == parent->lchd];
    auto f = sibling && sibling->red ? beforeErase4 : beforeErase5;
    f(root, tree, sibling, parent);
  }

  //   PB         SR        SB
  //  /  \       /  \      /  \
  // TB  SR  -> PB  YB -> PR  YB
  //    /  \   /  \      /  \
  //   XB  YB TB  XB    TB  XB
  void beforeErase4(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept {
    bool right = tree == parent->rchd;
    auto x = sibling->children[right];
    bitree::rotate(parent, right);
    sibling->red = false;
    parent->red = true;
    if (root == parent) {
      root = sibling;
    }
    beforeErase5(root, tree, x, parent);
  }

  static void beforeErase6(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept;
  static void beforeErase7(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept;
  static void beforeErase8(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept;

  void beforeErase5(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept {
    void (*f)(RBTree *&, RBTree *, RBTree *, RBTree *) noexcept;
    bool right = tree == parent->rchd;
    if (auto x = sibling->children[right]; x && x->red) {
      f = beforeErase6;
    } else if (auto y = sibling->children[!right]; y && y->red) {
      f = beforeErase7;
    } else {
      f = beforeErase8;
    }
    f(root, tree, sibling, parent);
  }

  //   PC        PC         __XR__        __XC__
  //  /  \      /  \       /      \      /      \_
  // TB  SB -> TB  XR  -> PC      SB -> PB      SB
  //    /         /  \   /  \    /     /  \    /
  //   XR        UB  SB TB  UB  VB    TB  UB  VB
  //  /  \          /
  // UB  VB        VB
  void beforeErase6(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept {
    bool right = node == 
  }
}
