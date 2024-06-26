#include"zlt/stack.h"

void zltStackPush(void *k, const void *data, size_t size) {
  memcpy(zltStackMemb(k, top), data, size);
  zltStackMemb(k, top) += size;
  zltStackMemb(k, left) -= size;
}

bool zltStackReCapacity(void *k, size_t capacity) {
  size_t size = zltStackSize(k);
  void *data = realloc(zltStackMemb(k, data), capacity);
  if (!data) {
    return false;
  }
  zltStackMemb(k, data) = data;
  if (size > capacity) {
    size = capacity;
  }
  zltStackMemb(k, top) = data + size;
  zltStackMemb(k, left) = capacity - size;
  return true;
}
