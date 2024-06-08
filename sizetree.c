#include"zlt/sizetree.h"

int zltSizeTreeCmpForFind(size_t data, const void *tree) {
  if (data < zltSizeTreeMemb(tree, value)) {
    return -1;
  }
  if (data > zltSizeTreeMemb(tree, value)) {
    return 1;
  }
  return 0;
}
