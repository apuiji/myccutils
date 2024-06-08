#include"zlt/23tree.h"

void zlt23TreeClean(void *tree, zlt23TreeDtor *dtor) {
  if (!tree) {
    return;
  }
  void *lchd = zlt23TreeMemb(tree, lchd);
  void *mchd = zlt23TreeMemb(tree, mchd);
  void *rchd = zlt23TreeMemb(tree, rchd);
  dtor(tree);
  zlt23TreeClean(lchd, dtor);
  zlt23TreeClean(mchd, dtor);
  zlt23TreeClean(rchd, dtor);
}

void *zlt23TreeMostSide(const void *tree, int side) {
  void *child = zlt23TreeMemb(tree, children)[side];
  if (child) {
    return zlt23TreeMostSide(child, side);
  }
  return zlt23TreeMemb(tree, data)[side];
}

// iterators begin
static void *nextLR1(zlt23TreeIter *it);

void *zlt23TreeNextLR(zlt23TreeIter *it) {
  if (it->dataIndex == 2) {
    return nextLR1(it);
  }
  ++it->dataIndex;
  void *d = zlt23TreeMemb(it->tree, data)[it->dataIndex];
  if (!d) {
    return zlt23TreeNextLR(it);
  }
  return d;
}

void *nextLR1(zlt23TreeIter *it) {
  void *p = zlt23TreeMemb(it->tree, parent);
  if (!p) {
    return NULL;
  }
  if (it->tree == zlt23TreeMemb(p, rchd)) {
    it->tree = p;
    return nextLR1(it);
  }
  if (it->tree == zlt23TreeMemb(p, mchd)) {
    it->tree = zlt23TreeMemb(p, rchd);
  } else {
    it->tree = zlt23TreeMemb(p, mchd);
  }
  it->dataIndex = 0;
  return zlt23TreeNextLR(it);
}

static void *nextRL1(zlt23TreeIter *it);

void *zlt23TreeNextRL(zlt23TreeIter *it) {
  if (it->dataIndex == 0) {
    return nextRL1(it);
  }
  --it->dataIndex;
  void *d = zlt23TreeMemb(it->tree, data)[it->dataIndex];
  if (!d) {
    return zlt23TreeNextRL(it);
  }
  return d;
}

void *nextRL1(zlt23TreeIter *it) {
  void *p = zlt23TreeMemb(it->tree, parent);
  if (!p) {
    return NULL;
  }
  if (it->tree == zlt23TreeMemb(p, lchd)) {
    it->tree = p;
    return nextRL1(it);
  }
  if (it->tree == zlt23TreeMemb(p, mchd)) {
    it->tree = zlt23TreeMemb(p, lchd);
  } else {
    it->tree = zlt23TreeMemb(p, mchd);
  }
  it->dataIndex = 2;
  return zlt23TreeNextRL(it);
}
// iterators end

// find operations begin
void *zlt23TreeFind(const void *tree, zlt23TreeCmpForFind *cmp, const void *data) {
  if (!tree) {
    return NULL;
  }
  void *datum = zlt23TreeMemb(tree, datum0);
  int i = cmp(data, datum);
  if (!i) {
    return datum;
  }
  if (i < 0) {
    tree = zlt23TreeMemb(tree, lchd);
    goto A;
  }
  datum = zlt23TreeMemb(tree, datum1);
  if (!datum) {
    tree = zlt23TreeMemb(tree, rchd);
    goto A;
  }
  i = cmp(data, datum);
  if (!i) {
    return datum;
  }
  if (i < 0) {
    tree = zlt23TreeMemb(tree, mchd);
    goto A;
  }
  tree = zlt23TreeMemb(tree, rchd);
  A:
  return zlt23TreeFind(tree, cmp, data);
}

void **zlt23TreeFindForInsert(void *tree, zlt23TreeCmpForFind *cmp, const void *data) {
  void **datum = &zlt23TreeMemb(tree, datum0);
  int i = cmp(data, *datum);
  if (!i) {
    return datum;
  }
  void *child;
  if (i < 0) {
    child = zlt23TreeMemb(tree, lchd);
    if (child) {
      goto A;
    }
  }
  datum = &zlt23TreeMemb(tree, datum1);
  if (!*datum) {
    return datum;
  }
  i = cmp(data, *datum);
  if (!i) {
    return datum;
  }
  A:
  return zlt23TreeFindForInsert(child, cmp, data);
}
// find operations end
