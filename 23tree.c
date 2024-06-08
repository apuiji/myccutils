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
static void *nxy1(const void *tree, int xy);

void *zlt23TreeNXY(const void *tree, int xy) {
  void *x = zlt23TreeMemb(tree, children)[xy];
  if (x) {
    return x;
  }
  return nxy1(tree, xy);
}

void *nxy1(const void *tree, int xy) {
  zlt23Tree *parent = (zlt23Tree *) zlt23TreeMemb(tree, parent);
  if (!parent) {
    return NULL;
  }
  if (tree == parent->children[xy]) {
    return parent->mchd;
  }
  if (tree == parent->mchd) {
    return parent->children[!xy];
  }
  return nxy1(parent, xy);
}

static void *xny1(const void *tree, int xy);

void *zlt23TreeXNY(const void *tree, int xy) {
  void *y = zltBiTreeMemb(tree, children)[!xy];
  if (y) {
    return zltBiTreeMostSide(y, xy);
  }
  return xny1(tree, xy);
}

void *xny1(const void *tree, int xy) {
  zltBiTree *parent = (zltBiTree *) zltBiTreeMemb(tree, parent);
  if (!parent) {
    return NULL;
  }
  if (tree == parent->children[xy]) {
    return parent;
  }
  return xny1(parent, xy);
}
