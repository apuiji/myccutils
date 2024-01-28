#pragma once

#include"rbtree.hh"
#include"xyz.hh"

namespace zlt {
  template<class K, class T>
  struct MyMapNode: rbtree::Node, std::pair<K, T> {
    MyMapNode(const K &k): pair(k, T()) {}
    MyMapNode(K &&k) noexcept: pair(std::move(k), T()) {}
  };

  template<class K, class T>
  int clean(MyMapNode<K, T> *node) noexcept {
    if (!node) [[unlikely]] {
      return 0;
    }
    clean(node->lchild);
    clean(node->rchild);
    delete node;
    return 0;
  }

  template<class K, class T, class Comp, class U>
  MyMapNode<K, T> *find(MyMapNode<K, T> *node, const U &u, const Comp &comp) noexcept {
    if (!node) [[unlikely]] {
      return nullptr;
    }
    int diff = comp(u, node->first);
    if (diff) {
      return find(node->children[diff > 0], u, comp);
    } else {
      return node;
    }
  }

  template<class K, class T, class Comp, class U>
  MapMapNode<K, T> *&find

  template<class K, class T, class Comp = Compare>
  struct MyMap {
    using Node = MyMapNode<K, T>;
    template<bool Right>
    using Iterator = rbtree::Iterator<Node, Right>;
    Comp comp;
    Node *root;
    MyMap(Comp &&comp = {}) noexcept: comp(std::move(comp)) {}
    ~MyMap() noexcept {
      clean(root);
    }
    Iterator<true> begin() const noexcept {
      if (root) {
        return mostLeft(root);
      } else {
        return {};
      }
    }
    Iterator<true> end() const noexcept {
      return {};
    }
    Iterator<false> rbegin() const noexcept {
      if (root) {
        return mostRight(root);
      } else {
        return {};
      }
    }
    Iterator<false> rend() const noexcept {
      return {};
    }
  };
}
