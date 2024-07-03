#include"zlt/strbuild.h"

bool zltStrBuildWrite(zltStack *dest, zltStack *buf, const void *data, size_t size) {
  if (!size) {
    return true;
  }
  if (size <= buf->left) {
    zltStackPush(buf, data, size);
  }
}

bool zltStrBuildFlush(zltStack *dest, zltStack *buf) {
  // TODO
  return false;
}
