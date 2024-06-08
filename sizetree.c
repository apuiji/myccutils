#include"zlt/sizetree.h"

int zltSizeTreeCmpForFind(const void *value, const void *tree) {
  if (zltPointTo(value, size_t) < zltSizeTreeMemb(tree, value)) {
    return -1;
  }
  if (zltPointTo(value, size_t) > zltSizeTreeMemb(tree, value)) {
    return 1;
  }
  return 0;
}
