#pragma once

#include"rbtree.hh"
#include"xyz.hh"

namespace zlt::myset {
  template<class T>
  struct Node: rbtree::Node {
    T value;
    Node(const T &value): value(value) {}
    Node(T &&value) noexcept: value(std::move(value)) {}
  };

  template<class T>
  int clean(Node<T> *node) noexcept {
    if (!node) [[unlikely]] {
      return 0;
    }
    clean(static_cast<Node<T> *>(node->lchild));
    clean(static_cast<Node<T> *>(node->rchild));
    delete node;
    return 0;
  }

  template<class T, class Comp, class U>
  Node<T> *find(Node<T> *node, const Comp &comp, U &&u) noexcept {
    if (!node) [[unlikely]] {
      return nullptr;
    }
    int diff = comp(std::forward<U>(u), node->value);
    if (diff) {
      auto next = node->children[diff > 0];
      return find(static_cast<Node<T> *>(next), comp, std::forward<U>(u));
    } else {
      return node;
    }
  }

  template<class T, class Comp, class U>
  static inline const Node<T> *find(const Node<T> *node, const Comp &comp, U &&u) noexcept {
    return find(const_cast<Node<T> *>(node), comp, std::forward<U>(u));
  }

  /// @param[out] parent initialized by null, the parent node of found
  /// @return not null when already exists
  template<class T, class Comp, class U>
  rbtree::Node *&findToInsert(rbtree::Node *&parent, rbtree::Node *&node, const Comp &comp, U &&u) noexcept {
    if (!node) [[unlikely]] {
      return node;
    }
    int diff = comp(std::forward<U>(u), static_cast<Node<T> *>(node)->value);
    if (diff) {
      parent = node;
      auto &next = node->children[diff > 0];
      return findToInsert<T>(parent, next, comp, std::forward<U>(u));
    } else {
      return node;
    }
  }

  template<class T, class Comp, class U>
  Node<T> *insert(rbtree::Node *&node, const Comp &comp, U &&u) {
    rbtree::Node *parent = nullptr;
    auto &a = findToInsert<T>(parent, node, comp, std::forward<U>(u));
    if (!a) {
      a = new Node<T>(std::forward<U>(u));
      a->parent = parent;
    }
    return static_cast<Node<T> *>(a);
  }

  template<class T, class Comp, class U, class V>
  Node<T> *insert(rbtree::Node *&node, const Comp &comp, U &&u, V &&v) {
    rbtree::Node *parent = nullptr;
    auto &a = findToInsert<T>(parent, node, comp, std::forward<U>(u));
    if (!a) {
      a = new Node<T>(std::forward<V>(v));
      a->parent = parent;
    }
    return static_cast<Node<T> *>(a);
  }

  template<class T, bool Right>
  using Iterator = rbtree::Iterator<Node<T>, Right>;

  template<class T, class Comp = Compare>
  struct Set {
    Comp comp;
    rbtree::Node *root;
    Set(Comp &&comp = {}) noexcept: comp(std::move(comp)) {}
    ~Set() noexcept {
      clean(static_cast<Node<T> *>(root));
    }
    Iterator<T, true> begin() const noexcept {
      if (root) {
        return mostLeft(root);
      } else {
        return {};
      }
    }
    Iterator<T, true> end() const noexcept {
      return {};
    }
    Iterator<T, false> rbegin() const noexcept {
      if (root) {
        return mostRight(root);
      } else {
        return {};
      }
    }
    Iterator<T, false> rend() const noexcept {
      return {};
    }
  };

  template<class T, class Comp, class U>
  static inline Node<T> *find(Set<T, Comp> &s, U &&u) noexcept {
    return find(static_cast<Node<T> *>(s.root), s.comp, std::forward<U>(u));
  }

  template<class T, class Comp, class U>
  static inline const Node<T> *find(const Set<T, Comp> &s, U &&u) noexcept {
    return find(static_cast<const Node<T> *>(s.root), s.comp, std::forward<U>(u));
  }

  template<class T, class Comp, class U>
  static inline auto insert(Set<T, Comp> &s, U &&u) {
    return insert<T>(s.root, s.comp, std::forward<U>(u));
  }

  template<class T, class Comp, class U, class V>
  static inline auto insert(Set<T, Comp> &s, U &&u, V &&v) {
    return insert<T>(s.root, s.comp, std::forward<U>(u), std::forward<V>(v));
  }
}
