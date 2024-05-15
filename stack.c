#include"stack.h"

bool zltStackPush(void *k, const void *data, size_t size) {
  if (zltStackMemb(k, left) < size) {
    return false;
  }
  memcpy(zltStackMemb(k, top), data, size);
  zltStackMemb(k, top) += size;
  zltStackMemb(k, left) -= size;
  return true;
}

bool zltStackRealloc(void *k, size_t size) {
  size_t distance = zltStackMemb(k, top) - zltStackMemb(k, data);
  void *data = realloc(zltStackMemb(k, data), size);
  if (!data) {
    return false;
  }
  zltStackMemb(k, data) = data;
  if (distance > size) {
    distance = size;
  }
  zltStackMemb(k, top) = data + distance;
  zltStackMemb(k, left) = size - distance;
  return true;
}
