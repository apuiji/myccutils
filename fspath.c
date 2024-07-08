#include"zlt/fspath.h"

static zltString parent(const char *begin, zltString src, zltString sep);

zltString zltFsPathParent(zltString path, zltString sep) {
  return parent(path.data, path, sep);
}

zltString parent(const char *begin, zltString src, zltString sep) {
  zltString s = zltStrFindStr(src, sep, strncmp);
  if (!s.size) {
    return zltStrMakeBE(begin, src.data);
  }
  return parent(begin, s, sep);
}

zltString zltFsPathName(zltString path, zltString sep) {
  zltString s = zltStrFindStr(path, sep);
  if (!s.size) {
    return path;
  }
  return zltFsPathName(s, sep);
}
