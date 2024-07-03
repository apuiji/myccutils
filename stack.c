#include<stdlib.h>
#include"zlt/stack.h"

void zltStackPush(zltStack *k, const void *data, size_t size) {
  memcpy(k->top, data, size);
  k->top += size;
  k->left -= size;
}

bool zltStackReCapacity(zltStack *k, size_t capacity) {
  size_t size = zltStackSize(k);
  void *data = realloc(k->data, capacity);
  if (!data) {
    return false;
  }
  k->data = data;
  if (size > capacity) {
    size = capacity;
  }
  k->top = (char *) data + size;
  k->left = capacity - size;
  return true;
}
