#include"rbtree.hh"

using namespace std;

namespace zlt::rbtree {
  Node *mostLeft(Node *node) noexcept {
    return node->lchild ? mostLeft(node->lchild) : node;
  }

  Node *mostRight(Node *node) noexcept {
    return node->rchild ? mostRight(node->rchild) : node;
  }

  Node *mostTop(Node *node) noexcept {
    return node->parent ? mostTop(node->parent) : node;
  }

  Node *nextLeft(const Node *node) noexcept {
    if (node->lchild) {
      return mostRight(node->lchild);
    }
    if (node == node->parent->rchild) {
      return node->parent;
    }
    return nullptr;
  }

  Node *nextRight(const Node *node) noexcept {
    if (node->rchild) {
      return mostLeft(node->rchild);
    }
    if (node == node->parent->lchild) {
      return node->parent;
    }
    return nullptr;
  }

  Node *leftRotate(Node *node) noexcept {
    auto parent = node->parent;
    auto rchild = node->rchild;
    node->parent = rchild;
    node->rchild = rchild->lchild;
    rchild->parent = parent;
    rchild->lchild = node;
    parent->children[node == parent->rchild] = rchild;
    return rchild;
  }

  Node *rightRotate(Node *node) noexcept {
    auto parent = node->parent;
    auto lchild = node->lchild;
    node->parent = lchild;
    node->lchild = lchild->rchild;
    lchild->parent = parent;
    lchild->rchild = node;
    parent->children[node == parent->rchild] = lchild;
    return lchild;
  }

  static int afterInsert1(Node *node) noexcept;

  int afterInsert(Node *&root, Node *node) noexcept {
    if (node->parent) {
      afterInsert1(node);
      root = mostTop(node);
    } else {
      node->red = false;
      root = node;
    }
    return 0;
  }

  int afterInsert1(Node *node) noexcept {
    auto parent = node->parent;
    if (!parent->red) {
      return 0;
    }
    auto gparent = parent;
    auto uncle = gparent->children[parent != gparent->rchild];
    if (uncle && uncle->red) {
      parent->red = false;
      uncle->red = false;
      gparent->red = true;
      return afterInsert1(gparent);
    }
    bool right = node == parent->rchild;
    if (right == (parent == gparent->rchild)) {
      //   GB         PB
      //  /  \       /  \
      // UB  PR ->  GR  NR
      //       \   /
      //       NR UB
      (right ? leftRotate : rightRotate)(gparent);
      parent->red = false;
      gparent->red = true;
    } else {
      //   GB        GB        NB
      //  /  \      /  \      /  \
      // PR  UB -> NR  UB -> PR  GR
      //  |       /                \
      //  NR     PR                UB
      if (right) {
        leftRotate(parent);
        rightRotate(gparent);
      } else {
        rightRotate(parent);
        leftRotate(gparent);
      }
      node->red = false;
      gparent->red = true;
    }
    return 0;
  }

  static bool beforeErase1(Node *&root, Node *node) noexcept;
  static int beforeErase2(Node *node) noexcept;

  int beforeErase(Node *&root, Node *node) noexcept {
    if (node->lchild && node->rchild) {
      swap(*node, *nextLeft(node->rchild));
    }
    if (beforeErase1(root, node)) {
      return 0;
    }
    if (!node->red) {
      beforeErase2(node);
    }
    if (auto parent = node->parent; parent) {
      root = mostTop(parent);
      parent->children[node == parent->rchild] = nullptr;
    } else {
      root = nullptr;
    }
    return 0;
  }

  bool beforeErase1(Node *&root, Node *node) noexcept {
    auto child = node->lchild;
    if (!child) {
      child = node->rchild;
      if (!child) {
        return false;
      }
    }
    auto parent = node->parent;
    child->parent = parent;
    child->red = false;
    if (parent) {
      parent->children[node == parent->rchild] = child;
    } else {
      root = child;
    }
    return true;
  }

  int beforeErase2(Node *node) noexcept {
    auto parent = node->parent;
    if (!parent) {
      return 0;
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
      return beforeErase2(node);
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
      return 0;
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
      return 0;
    }
    //     P?        P?
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
