#include"rbtree.hh"

using namespace std;

namespace zlt::rbtree {
  Node *mostLeftNN(Node *node) noexcept {
    return node->lchd ? mostLeftNN(node->lchd) : node;
  }

  Node *mostRightNN(Node *node) noexcept {
    return node->rchd ? mostRightNN(node->rchd) : node;
  }

  Node *mostTopNN(Node *node) noexcept {
    return node->parent ? mostTopNN(node->parent) : node;
  }

  Node *leftRotate(Node *node) noexcept {
    auto parent = node->parent;
    auto rchd = node->rchd;
    node->parent = rchd;
    node->rchd = rchd->lchd;
    rchd->parent = parent;
    rchd->lchd = node;
    if (parent) {
      (parent->lchd == node ? parent->lchd : parent->rchd) = rchd;
    }
    return rchd;
  }

  Node *rightRotate(Node *node) noexcept {
    auto parent = node->parent;
    auto lchd = node->lchd;
    node->parent = lchd;
    node->lchd = lchd->rchd;
    lchd->parent = parent;
    lchd->rchd = node;
    if (parent) {
      (parent->lchd == node ? parent->lchd : parent->rchd) = lchd;
    }
    return lchd;
  }

  Node *nextLeft(Node *node) noexcept {
    if (node->lchd) {
      return mostRightNN(node->lchd);
    }
    if (node == node->parent->rchd) {
      return node->parent;
    }
    return nullptr;
  }

  Node *nextRight(Node *node) noexcept {
    if (node->rchd) {
      return mostLeftNN(node->rchd);
    }
    if (node == node->parent->lchd) {
      return node->parent;
    }
    return nullptr;
  }

  static int afterInsert1(Node *node) noexcept;

  int afterInsert(Node *&root, Node *node) noexcept {
    afterInsert1(node);
    root = mostTopNN(node);
    return 0;
  }

  int afterInsert1(Node *node) noexcept {
    auto parent = node->parent;
    // root
    if (!parent) [[unlikely]] {
      node->red = false;
      return 0;
    }
    if (!parent->red) {
      return 0;
    }
    auto gparent = parent->parent;
    auto uncle = gparent->lchd == parent ? gparent->rchd : gparent->lchd;
    if (uncle->red) {
      parent->red = false;
      uncle->red = false;
      gparent->red = true;
      return afterInsert1(gparent);
    }
    bool right = node == parent->rchd;
    if (right == (parent == gparent->rchd)) {
      //   GB         PB
      //  /  \       /  \
      // UB  PR ->  GR  NR
      //       \   /
      //       NR UB
      rotate(gparent, !right);
      parent->red = false;
      gparent->red = true;
      return 0;
    } else {
      //   GB        GB        NB
      //  /  \      /  \      /  \
      // PR  UB -> NR  UB -> PR  GR
      //  |       /                \
      //  NR     PR                UB
      rotate(parent, !right);
      rotate(gparent, right);
      node->red = false;
      gparent->red = true;
      return 0;
    }
  }

  static int beforeErase1(Node *node) noexcept;

  int beforeErase(Node *&root, Node *node) noexcept {
    if (node->lchd && node->rchd) {
      swap(*node, *nextRight(node));
    }
    beforeErase1(node);
    auto parent = node->parent;
    if (parent) {
      root = mostTopNN(parent);
      parent->children[node == parent->rchd] = nullptr;
    } else {
      root = nullptr;
    }
    return 0;
  }

  static int beforeErase2(Node *node) noexcept;

  int beforeErase1(Node *node) noexcept {
    if (node->red) {
      return 0;
    }
    if (node->lchd) {
      rightRotate(node);
      node->lchd->red = false;
      return 0;
    }
    if (node->rchd) {
      leftRotate(node);
      node->rchd->red = false;
      return 0;
    }
    return beforeErase2(node);
  }

  int beforeErase2(Node *node) noexcept {
    auto parent = node->parent;
    if (!parent) [[unlikely]] {
      node->red = false;
      return 0;
    }
    bool right = node == parent->rchd;
    auto sibling = parent->children[right];
    if (sibling->red) {
      //     PB        SB
      //    /  \      /  \
      //   SR  NB -> XB  PR
      //  / |            | \
      // XB YB          YB NB
      rotate(parent, right);
      parent->red = true;
      sibling->red = false;
      return beforeErase2(node);
    }
    if (auto x = sibling->children[!right]; x && x->red) {
      //     P?        S?
      //    /  \      /  \
      //   SB  NB -> XB  PB
      //  /                \
      // XR                NB
      rotate(parent, right);
      sibling->red = parent->red;
      parent->red = false;
      x->red = false;
      return 0;
    }
    if (auto y = sibling->children[right]; y && y->red) {
      //   P?        P?        Y?
      //  /  \      /  \      /  \
      // SB  NB -> YR  NB -> SB  PB
      //  |       /                \
      //  YR     SB                NB
      rotate(sibling, !right);
      rotate(parent, right);
      y->red = parent->red;
      parent->red = false;
      return 0;
    }
    //     P?        PB
    //    /  \      /  \
    //   SB  NB -> SR  NB
    //  / |       / |
    // XB YB     XB YB
    sibling->red = true;
    if (parent->red) {
      parent->red = false;
      return 0;
    } else {
      return beforeErase2(parent);
    }
  }
}
