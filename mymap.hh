#pragma once

#include"rbtree.hh"
#include"xyz.hh"

namespace zlt::mymap {
  template<class K, class T>
  struct Node: rbtree::Node, std::pair<K, T> {
    Node(const K &k): std::pair<K, T>(k, T()) {}
    Node(K &&k) noexcept: std::pair<K, T>(std::move(k), T()) {}
  };

  template<class K, class T>
  int clean(Node<K, T> *node) noexcept {
    if (!node) [[unlikely]] {
      return 0;
    }
    clean(static_cast<Node<K, T> *>(node->lchild));
    clean(static_cast<Node<K, T> *>(node->rchild));
    delete node;
    return 0;
  }

  template<class K, class T, class Comp, class U>
  Node<K, T> *find(Node<K, T> *node, const Comp &comp, U &&u) noexcept {
    if (!node) [[unlikely]] {
      return nullptr;
    }
    int diff = comp(std::forward<U>(u), node->first);
    if (diff) {
      auto next = node->children[diff > 0];
      return find(static_cast<Node<K, T> *>(next), comp, std::forward<U>(u));
    } else {
      return node;
    }
  }

  template<class K, class T, class Comp, class U>
  static inline const Node<K, T> *find(const Node<K, T> *node, const Comp &comp, U &&u) noexcept {
    return find(const_cast<Node<K, T> *>(node), comp, std::forward<U>(u));
  }

  /// @param[out] parent initialized by null, the parent node of found
  /// @return not null when already exists
  template<class K, class T, class Comp, class U>
  rbtree::Node *&findToInsert(rbtree::Node *&parent, rbtree::Node *&node, const Comp &comp, U &&u) noexcept {
    if (!node) [[unlikely]] {
      return node;
    }
    int diff = comp(std::forward<U>(u), static_cast<Node<K, T> *>(node)->first);
    if (diff) {
      parent = node;
      auto &next = node->children[diff > 0];
      return findToInsert<K, T>(parent, next, comp, std::forward<U>(u));
    } else {
      return node;
    }
  }

  template<class K, class T, class Comp, class U>
  Node<K, T> *insert(Node<K, T> *&node, const Comp &comp, U &&u) {
    rbtree::Node *parent = nullptr;
    auto &a = findToInsert(parent, node, comp, std::forward<U>(u));
    if (!a) {
      a = new Node<K, T>(std::forward<U>(u));
      a->parent = parent;
    }
    return a;
  }

  template<class K, class T, class Comp, class U, class V>
  Node<K, T> *insert(rbtree::Node *&node, const Comp &comp, U &&u, V &&v) {
    rbtree::Node *parent = nullptr;
    auto &a = findToInsert<K, T>(parent, node, comp, std::forward<U>(u));
    if (!a) {
      a = new Node<K, T>(std::forward<V>(v));
      a->parent = parent;
    }
    return static_cast<Node<K, T> *>(a);
  }

  template<class K, class T, bool Right>
  using Iterator = rbtree::Iterator<Node<K, T>, Right>;

  template<class K, class T, class Comp = Compare>
  struct Map {
    Comp comp;
    rbtree::Node *root;
    Map(Comp &&comp = {}) noexcept: comp(std::move(comp)) {}
    ~Map() noexcept {
      clean(static_cast<Node<K, T> *>(root));
    }
    Iterator<K, T, true> begin() const noexcept {
      if (root) {
        return mostLeft(root);
      } else {
        return {};
      }
    }
    Iterator<K, T, true> end() const noexcept {
      return {};
    }
    Iterator<K, T, false> rbegin() const noexcept {
      if (root) {
        return mostRight(root);
      } else {
        return {};
      }
    }
    Iterator<K, T, false> rend() const noexcept {
      return {};
    }
    template<class U>
    T &operator [](U &&u) {
      return insert<K, T>(root, comp, std::forward<U>(u))->second;
    }
  };

  template<class K, class T, class Comp, class U>
  static inline Node<K, T> *find(Map<K, T, Comp> &m, U &&u) noexcept {
    return find(static_cast<Node<K, T> *>(m.root), m.comp, std::forward<U>(u));
  }

  template<class K, class T, class Comp, class U>
  static inline const Node<K, T> *find(const Map<K, T, Comp> &m, U &&u) noexcept {
    return find(static_cast<const Node<K, T> *>(m.root), m.comp, std::forward<U>(u));
  }

  template<class K, class T, class Comp, class U>
  static inline auto insert(Map<K, T, Comp> &m, U &&u) {
    return insert<K, T>(m.root, m.comp, std::forward<U>(u));
  }

  template<class K, class T, class Comp, class U, class V>
  static inline auto insert(Map<K, T, Comp> &m, U &&u, V &&v) {
    return insert<K, T>(m.root, m.comp, std::forward<U>(u), std::forward<V>(v));
  }
}
