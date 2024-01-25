#pragma once

#include<memory>
#include"xyz.hh"

namespace zlt {
  struct RBTreeNode {
    RBTreeNode *parent;
    RBTreeNode *lchd;
    RBTreeNode *rchd;
    bool red;
    RBTreeNode() noexcept: red(true) {}
  };

  template<class K, class T>
  struct MyMapNode: RBTreeNode, std::pair<K, T> {
    MyMapNode(const K &k) noexcept: pair(k, T()) {}
  };

  template<class K, class T>
  auto mostLeftNN(MyMapNode<K, T> *node) noexcept {
    return node->lchd ? mostLeftNN(node->lchd) : node;
  }

  template<class K, class T>
  static inline auto mostLeft(MyMapNode<K, T> *node) noexcept {
    return node ? mostLeftNN(node) : nullptr;
  }

  template<class K, class T>
  auto mostRightNN(MyMapNode<K, T> *node) noexcept {
    return node->rchd ? mostRightNN(node->rchd) : node;
  }

  template<class K, class T>
  static inline auto mostRight(MyMapNode<K, T> *node) noexcept {
    return node ? mostRightNN(node) : nullptr;
  }

  template<class K, class T>
  auto mostTopNN(MyMapNode<K, T> *node) noexcept {
    return node->parent ? mostTopNN(node->parent) : node;
  }

  template<class K, class T>
  static inline auto mostTop(MyMapNode<K, T> *node) noexcept {
    return node ? mostTopNN(node) : nullptr;
  }

  template<class K, class T>
  MyMapNode<K, T> *leftRotate(MyMapNode<K, T> *node) noexcept {
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

  template<class K, class T>
  MyMapNode<K, T> *rightRotate(MyMapNode<K, T> *node) noexcept {
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

  /// lrn
  template<class K, class T>
  struct MyMapIterator {
    using Node = MyMapNode<K, T>;
    using Iterator = MyMapIterator<K, T>;
    Node *node;
    bool operator ==(Iterator it) const noexcept {
      return node == it.node;
    }
    bool operator !=(Iterator it) const noexcept {
      return node != it.node;
    }
    Node &operator *() noexcept {
      return *node;
    }
    Node *operator ->() noexcept {
      return node;
    }
    Iterator next() const noexcept;
    Iterator &operator ++() noexcept {
      return *this = next();
    }
    Iterator operator ++(int) noexcept {
      return std::exchange(*this, next());
    }
  };

  template<class K, class T>
  MyMapIterator<K, T> MyMapIterator<K, T>::next() const noexcept {
    auto parent = node->parent;
    if (!parent) [[unlikely]] {
      return nullptr;
    }
    if (!parent->rchd || node == parent->rchd) {
      return parent;
    }
    return mostLeftNN(parent->rchd);
  }

  template<class K, class T, class Comparator = Compare, class Allocator = std::allocator<MyMapNode<K, T>>>
  requires requires (Comparator &&comparator, K &&a, K &&b) {
    { comparator(a, b) } -> std::signed_integral;
  }
  struct MyMap;

  template<class K, class T, class Comparator, class Allocator>
  MyMapNode<K, T> *findOrInsert(MyMap<K, T, Comparator, Allocator> &map, const K &k);

  template<class K, class T, class Comparator, class Allocator>
  struct MyMap {
    using Node = MyMapNode<K, T>;
    using Iterator = MyMapIterator<K, T>;
    Node *root;
    Comparator comparator;
    Allocator allocator;
    MyMap(Comparator &&comparator = {}, Allocator &&allocator = {}) noexcept:
    comparator(std::move(comparator)), allocator(std::move(allocator)) {}
    ~MyMap() noexcept;
    Iterator begin() {
      return mostLeft(root);
    }
    Iterator end() {
      return {};
    }
    T &operator [](const K &k) {
      return findOrInsert(*this, k)->second;
    }
  };

  template<class K, class T, class Comparator, class Allocator>
  MyMap<K, T, Comparator, Allocator>::~MyMap() noexcept {
    auto it = begin();
    auto end1 = end();
    while (it != end) {
      auto next = it.next();
      std::destroy(it.node);
      allocator.deallocate(it.node);
      it = next;
    }
  }

  template<class K, class T, class Comparator>
  MyMapNode<K, T> *&find(MyMapNode<K, T> *&root, const K &k, const Comparator &comparator) noexcept {
    if (!root) [[unlikely]] {
      return root;
    }
    int diff = cmp(k, root->first);
    return diff ? find(diff < 0 ? root->lchd : root->rchd, k, cmp) : root;
  }

  template<class K, class T, class Comparator, class Allocator>
  static inline MyMapNode<K, T> *find(MyMap<K, T, Comparator, Allocator> &map, const K &k) noexcept {
    return find(map.root, k, map.comparator);
  }

  template<class K, class T>
  int afterInsert(MyMapNode<K, T> *node) noexcept;

  template<class K, class T, class Comparator, class Allocator>
  MyMapNode<K, T> *&findOrInsert(MyMap<K, T, Comparator, Allocator> &map, const K &k) {
    auto &node = find(map.root, k, map.comparator);
    if (node) {
      return node;
    }
    node = map.allocator.allocate(1);
    std::construct(node, k);
    afterInsert(node);
    map.root = mostTopNN(node);
    return node;
  }

  template<class K, class T>
  int afterInsert(MyMapNode<K, T> *node) noexcept {
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

  template<class K, class T>
  int beforeErase(MyMapNode<K, T> *node) noexcept;

  template<class K, class T, class Comparator, class Allocator>
  int erase(MyMap<K, T, Comparator, Allocator> &map, const K &k) noexcept {
    auto *node = find(map, k);
    if (!node) {
      return 0;
    }
    if (node->rchd) {
      auto next = mostLeftNN(node->rchd);
      std::swap(node->parent, next->parent);
      std::swap(node->lchd, next->lchd);
      std::swap(node->rchd, next->rchd);
    }
    beforeErase(node);
    if (auto parent = node->parent; parent) {
      map.root = mostTopNN(parent);
      (parent->lchd == node ? parent->lchd : parent->rchd) = nullptr; 
    } else {
      map.root = nullptr;
    }
    std::destroy(node);
    map.allocator.deallocate(node);
    return 0;
  }

  template<class K, class T>
  int beforeErase(MyMapNode<K, T> *node) noexcept {
    if (node->red) {
      return 0;
    }
    auto parent = node->parent;
    if (!parent) [[unlikely]] {
      return 0;
    }
    if (parent->red) {
      //   PR        SB
      //  /  \  ->  /
      // NB  SB    PR
      //          /
      //         NB
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
    if (!(x && x->red) && !(y && y->red)) {
      //   P
      //  / \
      // NB SB
      //    | \
      //   XB YB
      sibling->red = true;
      if (parent->red) {
        parent->red = false;
        return 0;
      } else {
        return beforeErase(parent);
      }
    }
    {
      //   P
      //  / \
      // NB SB
      //    | \
      //   XB YR
    }
  }
}
