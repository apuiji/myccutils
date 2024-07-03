#include<string.h>
#include"zlt/strtree.h"

static inline int cmpForFind(const zltString *s, const zltStrTree *tree) {
  if (s->size < tree->value.size) {
    return -1;
  }
  if (s->size > tree->value.size) {
    return 1;
  }
  return strncmp(s->data, tree->value.data, s->size);
}

int zltStrTreeCmpForFind(const void *data, const zltBiTree *tree) {
  return cmpForFind((const zltString *) data, (const zltStrTree *) tree);
}
