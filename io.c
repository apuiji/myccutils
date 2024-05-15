#include"io.h"

void zltFputs(FILE *dest, zltString s) {
  const char *it = s.data;
  size_t left = s.size;
  for (; left; ++it, --left) {
    fputc(*it, dest);
  }
}
