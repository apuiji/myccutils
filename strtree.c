#include<string.h>
#include"strtree.h"

int zltStrTreeCmpForFind(const void *data, const void *tree) {
  if (zltStrMemb(data, size) < zltStrTreeMemb(tree, value).size) {
    return -1;
  }
  if (zltStrMemb(data, size) > zltStrTreeMemb(tree, value).size) {
    return 1;
  }
  return strncmp(zltStrMemb(data, data), zltStrTreeMemb(tree, value).data, zltStrMemb(data, size));
}
