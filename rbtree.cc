#include"rbtree.hh"

namespace zlt {
  RBTreeNode *mostLeftNN(RBTreeNode *node) noexcept {
    return node->lchd ? mostLeftNN(node->lchd) : node;
  }

  RBTreeNode *mostRightNN(RBTreeNode *node) noexcept {
    return node->rchd ? mostRightNN(node->rchd) : node;
  }

  RBTreeNode *mostTopNN(RBTreeNode *node) noexcept {
    return node->parent ? mostTopNN(node->parent) : node;
  }

  RBTreeNode *leftRotate(RBTreeNode *node) noexcept {
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

  RBTreeNode *rightRotate(RBTreeNode *node) noexcept {
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

  RBTreeIterator next(const RBTreeIterator &it) noexcept {
    auto parent = it.node->parent;
    if (!parent) [[unlikely]] {
      return nullptr;
    }
    if (!parent->rchd || it.node == parent->rchd) {
      return parent;
    }
    return mostLeftNN(parent->rchd);
  }

  int afterInsert(RBTreeNode *node) noexcept {
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
      return afterInsert(gparent);
    }
    if (node == parent->lchd) {
      if (parent == gparent->lchd) {
        //     GB        PR
        //    /  \      /  \
        //   PR  UB -> NB  GB
        //  / |            | \
        // NR SB          SB UB
        rightRotate(gparent);
        node->red = false;
        return afterInsert(parent);
      } else {
        leftRotate(parent);
        rightRotate(gparent);
        parent->red = false;
        return afterInsert(node);
      }
    } else {
      if (parent == gparent->rchd) {
        leftRotate(gparent);
        node->red = false;
        return afterInsert(parent);
      } else {
        rightRotate(parent);
        leftRotate(gparent);
        parent->red = false;
        return afterInsert(node);
      }
    }
  }

  int beforeErase(RBTreeNode *node) noexcept {
    if (node->red) {
      return 0;
    }
    auto parent = node->parent;
    if (!parent) [[unlikely]] {
      return 0;
    }
    auto sibling = node == parent->lchd ? parent->rchd : parent->lchd;
    if (sibling->red) {
      //   PB       SB
      //  /  \ ->  /
      // NB  SR   PR
      //         /
      //        NB
      if (node == parent->lchd) {
        leftRotate(parent);
      } else {
        rightRotate(parent);
      }
      parent->red = true;
      sibling->red = false;
      return beforeErase(node);
    }
    auto x = sibling->lchd;
    auto y = sibling->rchd;
    if ((x && x->red) == (y && y->red)) {
      //   P
      //  / \
      // NB SB
      //    | \
      //   XR YR
      sibling->red = true;
      x->red = false;
      y->red = false;
      if (parent->red) {
        parent->red = false;
        return 0;
      } else {
        return beforeErase(parent);
      }
    }
    if (node == parent->lchd) {
      //   PR         SB
      //  /  \       /  \
      // NB  SB  -> PR  YR
      //     | \   / |
      //    XB YR NB XB
    }
  }
}
