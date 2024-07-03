#include<stddef.h>
#include"zlt/dblink.h"

void zltDbLinkErase(zltDbLink *link, zltDbLink *last) {
  link->prev->next = last->next;
  last->next->prev = link->prev;
  link->prev = NULL;
  last->next = NULL;
}
