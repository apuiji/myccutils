#include"zlt/fspath.h"

#ifdef WIN32

static zltString parent(const char *begin, const char *end);

zltString zltFsPathParent(zltString path) {
  if (path.size < zltFsPathSepLeng) {
    return path;
  }
  const char *end = path.data + path.size;
  if (zltStrEndsWith(path, zltStrMakeSta(zltFsPathSep))) {
    end -= zltFsPathSepLeng;
  }
  return parent(path.data, end);
}

zltString parent(const char *begin, const char *end) {
  ;
}

#else

zltString zltFsPathParent(zltString path) {
  // TODO
  return (zltString) {};
}

#endif
