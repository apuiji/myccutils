#include"zlt/strbuild.h"

zltString zltStrBuildWrite(zltStrBuild *sb, zltString src) {
  if (src.size <= sb->buf.left) {
    zltStackPushStr(&sb->buf, src);
    return zltStrEnd(src);
  }
  size_t n = sb->buf.left;
  zltStackPush(&sb->buf, src.data, n);
  src = zltStrForward(src, n);
  if (!zltStrBuildFlush(sb)) {
    return src;
  }
  return zltStrBuildWrite(sb, src);
}

bool zltStrBuildFlush(zltStrBuild *sb) {
  size_t n = zltStackSize(&sb->buf);
  if (n > sb->dest.left) {
    size_t m = zltStackSize(&sb->dest);
    if (!zltStackReCapacity(&sb->dest, m + n)) {
      return false;
    }
  }
  zltStackPushStr(&sb->dest, zltStrMake(sb->buf.data, n));
  zltStackPop(&sb->buf, n);
  return true;
}
