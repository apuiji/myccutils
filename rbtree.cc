#include"rbtree.hh"

using namespace std;

namespace zlt::rbtree {
  void RBTree::swap(RBTree &a) noexcept {
    BiTree::swap(a);
    std::swap(red, a.red);
  }

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
  static void beforeErase4(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept;
  static void beforeErase5(RBTree *&root, RBTree *tree, RBTree *sibling, RBTree *parent) noexcept;

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

  void beforeErase2(Node *node) noexcept {
    auto parent = node->parent;
    if (!parent) {
      return;
    }
    bool right = node == parent->rchild;
    auto sibling = parent->children[!right];
    if (sibling->red) {
      //     PB        SB
      //    /  \      /  \
      //   SR  NB -> XB  PR
      //  / |            | \
      // XB YB          YB NB
      (right ? rightRotate : leftRotate)(parent);
      parent->red = true;
      sibling->red = false;
      beforeErase2(node);
      return;
    }
    if (auto y = sibling->children[right]; y && y->red) {
      //     P1        P1        Y1
      //    /  \      /  \      /  \
      //   SB  NB -> YR  NB -> SB  PB
      //    |       /                \
      //    YR     SB                NB
      if (right) {
        leftRotate(sibling);
        rightRotate(parent);
      } else {
        rightRotate(sibling);
        leftRotate(parent);
      }
      y->red = parent->red;
      parent->red = false;
      return;
    }
    if (auto x = sibling->children[!right]; x && x->red) {
      //     P1        S1
      //    /  \      /  \
      //   SB  NB -> XB  PB
      //  /                \
      // XR                NB
      (right ? rightRotate : leftRotate)(parent);
      sibling->red = parent->red;
      parent->red = false;
      x->red = false;
      return;
    }
    //     P?        P?
    //    /  \      /  \
    //   SB  NB -> SR  NB
    //  / |       / |
    // XB YB     XB YB
    sibling->red = true;
    if (parent->red) {
      parent->red = false;
    } else {
      beforeErase2(parent);
    }
  }
}
