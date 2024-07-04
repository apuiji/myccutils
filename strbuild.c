#include"zlt/strbuild.h"

zltString zltStrBuildWrite(zltStack *dest, zltStack *buf, zltString src) {
  if (src.size <= buf->left) {
    zltStackPushStr(buf, src);
    return zltStrEnd(src);
  }
  size_t n = buf->left;
  zltStackPush(buf, src.data, n);
  src = zltStrForward(src, n);
  if (!zltStrBuildFlush(dest, buf)) {
    return src;
  }
  return zltStrBuildWrite(dest, buf, src);
}

bool zltStrBuildFlush(zltStack *dest, zltStack *buf) {
  size_t n = zltStackSize(buf);
  if (n > dest->left) {
    size_t m = zltStackSize(dest);
    if (!zltStackReCapacity(dest, m + n)) {
      return false;
    }
  }
  zltStackPushStr(dest, zltStrMake(buf->data, n));
  return true;
}
