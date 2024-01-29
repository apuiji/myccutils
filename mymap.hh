#pragma once

#include"rbtree.hh"
#include"xyz.hh"

namespace zlt {
  template<class K, class T>
  struct MyMapNode: rbtree::Node, std::pair<K, T> {
    using Pair = std::pair<K, T>;
    MyMapNode(const K &k): Pair(k, T()) {}
    MyMapNode(K &&k) noexcept: Pair(std::move(k), T()) {}
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
  MyMapNode<K, T> *find(MyMapNode<K, T> *node, const Comp &comp, const U &u) noexcept {
    if (!node) [[unlikely]] {
      return nullptr;
    }
    int diff = comp(u, node->first);
    if (diff) {
      auto next = node->children[diff > 0];
      return find(static_cast<MyMapNode<K, T> *>(next), comp, u);
    } else {
      return node;
    }
  }

  template<class K, class T, class Comp, class U>
  static inline const MyMapNode<K, T> *find(const MyMapNode<K, T> *node, const Comp &comp, const U &u) noexcept {
    return find(const_cast<MyMapNode<K, T> *>(node), comp, u);
  }

  /// @param[out] parent initialized by null, the parent node of found
  /// @return not null when already exists
  template<class K, class T, class Comp, class U>
  MyMapNode<K, T> *&findToInsert(rbtree::Node *&parent, MyMapNode<K, T> *&node, const Comp &comp, const U &u) noexcept {
    if (!node) [[unlikely]] {
      return node;
    }
    int diff = comp(u, node->first);
    if (diff) {
      parent = node;
      auto &next = node->children[diff > 0];
      return findToInsert(parent, static_cast<MyMapNode<K, T> *&>(next), comp, u);
    } else {
      return node;
    }
  }

  template<class K, class T, class Comp, class U, class V>
  MyMapNode<K, T> *insert(MyMapNode<K, T> *&node, const Comp &comp, const U &u, V &&v) {
    rbtree::Node *parent = nullptr;
    auto &a = findToInsert(parent, node, comp, u);
    if (!a) {
      a = new MyMapNode<K, T>(std::forward<V>(v));
      a->parent = parent;
    }
    return a;
  }

  template<class K, class T, class Comp, class U>
  MyMapNode<K, T> *insert(MyMapNode<K, T> *&node, const Comp &comp, U &&u) {
    return insert(node, comp, u, std::forward<U>(u));
  }

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
    template<class U>
    T &operator [](U &&u) {
      return insert(root, comp, std::forward<U>(u))->second;
    }
  };

  template<class K, class T, class Comp, class U>
  static inline auto find(MyMap<K, T, Comp> &m, const U &u) noexcept {
    return find(m.root, m.comp, u);
  }

  template<class K, class T, class Comp, class U>
  static inline auto find(const MyMap<K, T, Comp> &m, const U &u) noexcept {
    return find(m.root, m.comp, u);
  }

  template<class K, class T, class Comp, class U, class V>
  static inline auto insert(MyMap<K, T, Comp> &m, const U &u, V &&v) {
    return insert(m.root, m.comp, u, std::forward<V>(v));
  }

  template<class K, class T, class Comp, class U>
  static inline auto insert(MyMap<K, T, Comp> &m, U &&u) {
    return insert(m.root, m.comp, u, std::forward<U>(u));
  }
}
