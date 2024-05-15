#include"ptrtree.h"

int zltPtrTreeCmpForFind(const void *data, const void *tree) {
  if (data < zltPtrTreeMemb(tree, value)) {
    return -1;
  }
  if (data > zltPtrTreeMemb(tree, value)) {
    return 1;
  }
  return 0;
}
