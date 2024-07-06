#include"zlt/rbtree1.h"
#include"zlt/xyz.h"

int zltDoubleTreeCmpForFind(const void *value, const zltBiTree *tree) {
  if (*(double *) value < zltMemberOf(tree, zltDoubleTree, value)) {
    return -1;
  }
  if (*(double *) value > zltMemberOf(tree, zltDoubleTree, value)) {
    return 1;
  }
  return 0;
}

int zltPtrTreeCmpForFind(const void *value, const zltBiTree *tree) {
  return value - zltMemberOf(tree, zltPtrTree, value);
}

int zltSizeTreeCmpForFind(const void *value, const zltBiTree *tree) {
  if (*(size_t *) value < zltMemberOf(tree, zltSizeTree, value)) {
    return -1;
  }
  if (*(size_t *) value > zltMemberOf(tree, zltSizeTree, value)) {
    return 1;
  }
  return 0;
}

static inline int strTreeCmpForFind(const zltString *s, const zltStrTree *tree) {
  if (s->size < tree->value.size) {
    return -1;
  }
  if (s->size > tree->value.size) {
    return 1;
  }
  return strncmp(s->data, tree->value.data, s->size);
}

int zltStrTreeCmpForFind(const void *data, const zltBiTree *tree) {
  return strTreeCmpForFind((const zltString *) data, (const zltStrTree *) tree);
}
