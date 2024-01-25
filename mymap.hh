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
}
