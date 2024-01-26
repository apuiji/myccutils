#pragma once

#include<memory>
#include"rbtree.hh"

namespace zlt {
  template<class K, class T>
  struct MyMapNode: std::pair<K, T>, rbtree::Node {
    MyMapNode(const K &k) noexcept: pair(k, T()) {}
    MyMapNode(K &&k) noexcept: pair(std::move(k), T()) {}
  };

  template<class K, class T, class Allocator>
  int clear(MyMapNode<K, T> *node, Allocator &allocator) noexcept {
    if (!node) [[unlikely]] {
      return 0;
    }
    clear(node->lchd, allocator);
    clear(node->rchd, allocator);
    std::destroy(node);
    allocator.deallocate(node);
    return 0;
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
    Comparator comparator;
    Allocator allocator;
    Node *root;
    MyMap(Comparator &&comparator = {}, Allocator &&allocator = {}) noexcept:
    comparator(std::move(comparator)), allocator(std::move(allocator)) {}
    ~MyMap() noexcept {
      clear(root, allocator);
    }
    rbtree::Iterator<Node> begin() const noexcept {
      return mostLeft(root);
    }
    rbtree::Iterator<Node> end() const noexcept {
      return {};
    }
    T &operator [](const K &k) {
      return findOrInsert(*this, k)->second;
    }
  };

  template<class K, class T, class Comparator>
  MyMapNode<K, T> *&find(MyMapNode<K, T> *&root, const K &k, const Comparator &comparator) noexcept {
    if (!root) [[unlikely]] {
      return nullptr;
    }
    int diff = comparator(k, root->first);
    return diff ? find(root->children[diff > 0], k, comparator) : root;
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
    node->parent = containerOf(node, rbtree::Node::);
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
