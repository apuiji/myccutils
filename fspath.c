#include"zlt/fspath.h"

#ifdef WIN32

static bool startsWithSep(zltString src) {
  return src.size && (*src.data == '/' || *src.data == '\\');
}

static bool endsWithSep(zltString src) {
  if (!src.size) {
    return false;
  }
  const char *c = src.data + src.size;
  return *c == '/' || *c == '\\';
}

static zltString parent(zltString path);

zltString zltFsPathParent(zltString path) {
  if (zltStrEndsWith(path, zltStrMakeSta(zltFsPathSep))) {
    return zltFsPathParent(zltStrEndBack(path, zltFsPathSepLeng));
  }
  return parent(path);
}

static bool startsWithSep(zltString src) {
  return zltStrStartsWith(path, zltStrMakeSta(zltFsPathSep));
}

zltString parent(zltString path) {
  zltString s = zltStrRevFindIf(path, startsWithSep);
}

#else

zltString zltFsPathParent(zltString path) {
  // TODO
  return (zltString) {};
}

#endif
