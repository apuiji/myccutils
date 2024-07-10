#include<ctype.h>
#include<math.h>
#include<string.h>
#include"zlt/string.h"

int zltIsDigitChar(int c, size_t base) {
  if (base >= 2 && base < 10) {
    return c >= '0' && c < '0' + base ? c - '0' : -1;
  }
  if (base > 10 && base <= 36) {
    if (isdigit(c)) {
      return c - '0';
    }
    if (c >= 'A' && c < 'A' + base - 10) {
      return c - 'A' + 10;
    }
    if (c >= 'a' && c < 'a' + base - 10) {
      return c - 'a' + 10;
    }
    return -1;
  }
  return -1;
}

// comparisons begin
int zltStrCmp(zltString a, zltString b, strncmpFn *cmp) {
  if (a.size > b.size) {
    return -zltStrCmp(b, a, cmp);
  }
  int diff = cmp(a.data, b.data, a.size);
  if (diff > 0) {
    return 1;
  }
  if (diff < 0 || a.size < b.size) {
    return -1;
  }
  return 0;
}

static size_t strSame(size_t dest, const char *a, const char *b, size_t left, size_t once, strncmpFn *cmp);

size_t zltStrSame(zltString a, zltString b, strncmpFn *cmp) {
  return strSame(0, a.data, b.data, a.size < b.size ? a.size : b.size, 1 << 8, cmp);
}

size_t strSame(size_t dest, const char *a, const char *b, size_t left, size_t once, strncmpFn *cmp) {
  for (; left >= once && !cmp(a, b, once); a += once, b += once, left -= once) {
    dest += once;
  }
  once = once >> 1;
  return once ? strSame(dest, a, b, left, once, cmp) : dest;
}
// comparisons end

// trim operations begin
zltString zltStrTrimStart(zltString str) {
  const char *it = str.data;
  size_t left = str.size;
  for (; left && isspace(*it); ++it, --left) {
    // do nothing
  }
  return zltStrMake(it, left);
}

zltString zltStrTrimEnd(zltString str) {
  const char *it = str.data + str.size - 1;
  size_t left = str.size;
  for (; left && isspace(*it); --it, --left) {
    // do nothing
  }
  return zltStrMake(str.data, left);
}
// trim operations end

// find operations begin
zltString zltStrFindChar(zltString src, int c) {
  for (; src.size && *src.data != c; ++src.data, --src.size) {
    // do nothing
  }
  return src;
}

zltString zltStrFindStr(zltString src, zltString str, strncmpFn *cmp) {
  for (; src.size && zltStrStartsWith(src, str, cmp); ++src.data, --src.size) {
    // do nothing
  }
  return src;
}

zltString zltStrFindIf(zltString src, zltStrPredForFind *pred) {
  for (; src.size && !pred(src); ++src.data, --src.size) {
    // do nothing
  }
  return src;
}

zltString zltStrRevFindIf(zltString src, zltStrPredForFind *pred) {
  zltString s = zltStrEnd(src);
  for (; s.size < src.size && !pred(s); --s.data, ++s.size) {
    // do nothing
  }
  return zltStrMakeBE(src.data, s.data);
}

// kmp begin
static void kmpNextMake(size_t *dest, size_t n, const char *a, const char *b, size_t left);

void zltKMPNextMake(size_t *dest, zltString src) {
  *dest = 0;
  kmpNextMake(dest + 1, 0, src.data, src.data + 1, src.size - 1);
}

void kmpNextMake(size_t *dest, size_t n, const char *a, const char *b, size_t left) {
  if (!left) {
    return;
  }
  if (*a == *b) {
    *dest = ++n;
    kmpNextMake(dest + 1, n, a + 1, b + 1, left - 1);
    return;
  }
  kmpNextMake(dest, 0, a - n, b, left);
}

static zltString kmpFind(zltString src, zltString pat, size_t *nextv, int i);

zltString zltStrKMPFind(zltString src, zltString pat, size_t *nextv) {
  return kmpFind(src, pat, nextv, 0);
}

zltString kmpFind(zltString src, zltString pat, size_t *nextv, int i) {
  if (!src.size) {
    return src;
  }
  if (*src.data == pat.data[i]) {
    return kmpFind(zltStrForward(src, 1), pat, nextv, i + 1);
  }
  return kmpFind(src, pat, nextv, i ? nextv[i - 1] : 0);
}
// kmp end
// find operations end

void zltStrToCase(zltString dest, zltString src, tocaseFn *tocase) {
  for (; dest.size && src.size; ++dest.data, --dest.size, ++src.data, --src.size) {
    *dest.data = tocase(*src.data);
  }
}

// string to number operations begin
zltString zltStrToLong(long *dest, zltString src, size_t base, zltStrToULongFn *toULong) {
  if (!src.size) {
    return src;
  }
  if (*src.data == '+') {
    if (src.size == 1) {
      return src;
    }
    zltString s = toULong((unsigned long *) dest, zltStrForward(src, 1), base);
    if (src.size - s.size == 1) {
      return src;
    }
    return s;
  }
  if (*src.data == '-') {
    if (src.size == 1) {
      return src;
    }
    zltString s = toULong((unsigned long *) dest, zltStrForward(src, 1), base);
    if (src.size - s.size == 1) {
      return src;
    }
    *dest = -*dest;
    return s;
  }
  return toULong((unsigned long *) dest, src, base);
}

zltString zltStrToULong(unsigned long *dest, zltString src, size_t base) {
  for (; src.size; src = zltStrForward(src, 1)) {
    int i = zltIsDigitChar(*src.data, base);
    if (i < 0) {
      break;
    }
    *dest = (*dest * base) + i;
  }
  return src;
}

zltString zltStrToDouble(double *dest, zltString src, zltStrToUDoubleFn *toUDouble) {
  if (!src.size) {
    return src;
  }
  if (*src.data == '+') {
    if (src.size == 1) {
      return src;
    }
    zltString s = toUDouble(dest, zltStrForward(src, 1));
    if (src.size - s.size == 1) {
      return src;
    }
    return s;
  }
  if (*src.data == '-') {
    if (src.size == 1) {
      return src;
    }
    zltString s = toUDouble(dest, zltStrForward(src, 1));
    if (src.size - s.size == 1) {
      return src;
    }
    *dest = -*dest;
    return s;
  }
  return toUDouble(dest, src);
}

static zltString strtoud1(double *dest, zltString src);
static zltString strtoud2(double *dest, zltString src);
static zltString strtoud3(double *dest, zltString src);

zltString zltStrToUDouble(double *dest, zltString src) {
  zltString s = strtoud1(dest, src);
  return s.data ? strtoud3(dest, s) : src;
}

zltString strtoud1(double *dest, zltString src) {
  if (!src.size) {
    return zltStrMake(NULL, 0);
  }
  if (isdigit(*src.data)) {
    unsigned long l = 0;
    zltString s = zltStrToULong(&l, src, 10);
    *dest += l;
    return s.size && *s.data == '.' ? strtoud2(dest, zltStrForward(s, 1)) : s;
  }
  if (*src.data == '.' && src.size > 1 && isdigit(src.data[1])) {
    return strtoud2(dest, zltStrForward(src, 1));
  }
  return zltStrMake(NULL, 0);
}

zltString strtoud2(double *dest, zltString src) {
  unsigned long l = 0;
  zltString s = zltStrToULong(&l, src, 10);
  *dest += l / pow(10, src.size - s.size);
  return s;
}

zltString strtoud3(double *dest, zltString src) {
  if (src.size < 2) {
    return src;
  }
  if (!(*src.data == 'E' || *src.data == 'e')) {
    return src;
  }
  long e = 0;
  zltString s = zltStrToLong(&e, zltStrForward(src, 1), 10, zltStrToULong);
  if (src.size - s.size == 1) {
    return src;
  }
  *dest *= pow(10, e);
  return s;
}
// string to number operations end
