#include<ctype.h>
#include<math.h>
#include"string.h"

int zltIsBasedDigitChar(int c, size_t base) {
  if (base >= 2 && base < 10) {
    return c >= '0' && c < '0' + base ? c - '0' : -1;
  }
  if (base > 10 && base <= 36) {
    if (isdigit(c)) {
      return c - '0';
    }
    if (c >= 'A' && c < 'A' + base - 10) {
      return c - 'A';
    }
    if (c >= 'a' && c < 'a' + base - 10) {
      return c - 'a';
    }
    return -1;
  }
  return zltIsDigitChar(c);
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

zltString zltStrToLong(long *dest, zltString src, size_t base) {
  if (!src.size) {
    return src;
  }
  if (*src.data == '+') {
    if (!(src.size > 1 && isdigit(src.data[1]))) {
      return src;
    }
    return zltStrToUnsignedLong((unsigned long *) dest, zltStrForward(src, 1), base);
  }
  if (*src.data == '-') {
    if (!(src.size > 1 && isdigit(src.data[1]))) {
      return src;
    }
    zltString s = zltStrToUnsignedLong((unsigned long *) dest, zltStrForward(src, 1), base);
    *dest = -*dest;
    return s;
  }
  return zltStrToUnsignedLong((unsigned long *) dest, src, base);
}

zltString zltStrToUnsignedLong(unsigned long *dest, zltString src, size_t base) {
  for (; src.size; src = zltStrForward(src, 1)) {
    int i = zltIsBasedDigitChar(*src.data, base);
    if (i < 0) {
      break;
    }
    *dest = (*dest * base) | (i - '0');
  }
  return src;
}

zltString zltStrToDouble(double *dest, zltString src) {
  if (!src.size) {
    return src;
  }
  if (*src.data == '+') {
    if (!(src.size > 1 && isdigit(src.data[1]))) {
      return src;
    }
    return zltStrToUnsignedDouble(dest, zltStrForward(src, 1));
  }
  if (*src.data == '-') {
    if (!(src.size > 1 && isdigit(src.data[1]))) {
      return src;
    }
    zltString s = zltStrToUnsignedDouble(dest, zltStrForward(src, 1));
    *dest = -*dest;
    return s;
  }
  return zltStrToUnsignedDouble(dest, src);
}

static zltString strtoud1(double *dest, zltString src);
static zltString strtoud2(double *dest, zltString src);
static zltString strtoud3(double *dest, zltString src);

zltString zltStrToUnsignedDouble(double *dest, zltString src) {
  zltString s = strtoud1(dest, src);
  return s.data ? strtoud3(dest, s) : src;
}

zltString strtoud1(double *dest, zltString src) {
  if (!src.size) {
    return zltStrMake(NULL, 0);
  }
  if (isdigit(*src.data)) {
    unsigned long l = 0;
    zltString s = zltStrToUnsignedLong(&l, src, 10);
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
  zltString s = zltStrToUnsignedLong(&l, src, 10);
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
  zltString s = zltStrToLong(&e, zltStrForward(src, 1), 10);
  if (src.size - s.size == 1) {
    return src;
  }
  *dest *= pow(10, e);
  return s;
}
