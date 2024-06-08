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

bool zltStrEq(zltString a, zltString b) {
  return a.size == b.size && strncmp(a.data, b.data, a.size);
}

int zltStrCmp(zltString a, zltString b) {
  if (a.size > b.size) {
    return -zltStrCmp(b, a);
  }
  int diff = strncmp(a.data, b.data, a.size);
  if (diff > 0) {
    return 1;
  }
  if (diff < 0 || a.size < b.size) {
    return -1;
  }
  return 0;
}

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
  return zltStrMake(it, left);
}

zltString zltStrToLong(long *dest, zltString src, size_t base, zltStrToULongFn *toULong) {
  if (!src.size) {
    return src;
  }
  if (*src.data == '+') {
    if (!(src.size > 1 && zltIsDigitChar(src.data[1], base))) {
      return src;
    }
    return toULong((unsigned long *) dest, zltStrForward(src, 1), base);
  }
  if (*src.data == '-') {
    if (!(src.size > 1 && zltIsDigitChar(src.data[1], base))) {
      return src;
    }
    zltString s = toULong((unsigned long *) dest, zltStrForward(src, 1), base);
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
    if (!(src.size > 1 && isdigit(src.data[1]))) {
      return src;
    }
    return toUDouble(dest, zltStrForward(src, 1));
  }
  if (*src.data == '-') {
    if (!(src.size > 1 && isdigit(src.data[1]))) {
      return src;
    }
    zltString s = toUDouble(dest, zltStrForward(src, 1));
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
