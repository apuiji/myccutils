#include"xyz.h"

static inline void memSwapChar(char *a, char *b, size_t size) {
  for (; size; ++a, ++b, --size) {
    char c = *a;
    *a = *b;
    *b = c;
  }
}

static inline void memSwapInt(int *a, int *b, size_t size) {
  for (; size >= sizeof(int); ++a, ++b, size -= sizeof(int)) {
    int c = *a;
    *a = *b;
    *b = c;
  }
  memSwapChar((char *) a, (char *) b, size);
}

void zltMemSwap(void *a, void *b, size_t size) {
  memSwapInt((int *) a, (int *) b, size);
}
