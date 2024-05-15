#include"link.h"

void zltLinkClean(void *link, const void *end, zltLinkDtor *dtor) {
  if (link == end) {
    return;
  }
  void *next = zltLinkMemb(link, next);
  dtor(link);
  zltLinkClean(next, end, dtor);
}

void **zltLinkInsert(void **dest, void *link, void *last) {
  zltLinkMemb(last, next) = *dest;
  *dest = link;
  return &zltLinkMemb(last, next);
}

void *zltLinkErase(void **link, void *last) {
  void *a = *link;
  *link = zltLinkMemb(last, next);
  zltLinkMemb(last, next) = NULL;
  return a;
}
