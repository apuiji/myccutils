#include"zlt/sizetree.h"
#include"zlt/xyz.h"

int zltSizeTreeCmpForFind(const void *value, const zltBiTree *tree) {
  if (*(size_t *) value < zltMemberOf(tree, zltSizeTree, value)) {
    return -1;
  }
  if (*(size_t *) value > zltMemberOf(tree, zltSizeTree, value)) {
    return 1;
  }
  return 0;
}
