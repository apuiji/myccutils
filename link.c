#include"zlt/link.h"

void zltLinkClean(zltLink *link, zltLinkDtor *dtor) {
  while (link) {
    zltLink *next = link->next;
    dtor(link);
    link = next;
  }
}

zltLink *zltLinkFindUntil(const zltLink *link, const zltLink *end) {
  while (link->next != end) {
    link = link->next;
  }
  return (zltLink *) link;
}
