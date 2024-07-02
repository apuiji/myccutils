#include"zlt/bitree.h"
#include"zlt/xyz.h"

void zltBiTreeSwap(zltBiTree **root, zltBiTree *a, zltBiTree *b) {
  zltMemSwap(a, b, sizeof(zltBiTree));
  if (!a->parent) {
    *root = a;
  } else if (!b->parent) {
    *root = b;
  }
}

void zltBiTreeClean(zltBiTree *tree, zltBiTreeDtor *dtor) {
  if (!tree) {
    return;
  }
  zltBiTree *lchd = tree->lchd;
  zltBiTree *rchd = tree->rchd;
  dtor(tree);
  zltBiTreeClean(lchd, dtor);
  zltBiTreeClean(rchd, dtor);
}

zltBiTree *zltBiTreeMostSide(const zltBiTree *tree, int side) {
  zltBiTree *next = tree->children[side];
  return next ? zltBiTreeMostSide(next, side) : (zltBiTree *) tree;
}

// iterators begin
static zltBiTree *nxy1(const zltBiTree *tree, int xy);

zltBiTree *zltBiTreeNXY(const zltBiTree *tree, int xy) {
  zltBiTree *x = tree->children[xy];
  if (x) {
    return x;
  }
  zltBiTree *y = tree->children[!xy];
  if (y) {
    return y;
  }
  return nxy1(tree, xy);
}

zltBiTree *nxy1(const zltBiTree *tree, int xy) {
  zltBiTree *parent = tree->parent;
  if (!parent) {
    return NULL;
  }
  if (tree == parent->children[xy] && parent->children[!xy]) {
    return parent->children[!xy];
  }
  return nxy1(parent, xy);
}

static zltBiTree *xny1(const zltBiTree *tree, int xy);

zltBiTree *zltBiTreeXNY(const zltBiTree *tree, int xy) {
  zltBiTree *y = tree->children[!xy];
  if (y) {
    return zltBiTreeMostSide(y, xy);
  }
  return xny1(tree, xy);
}

zltBiTree *xny1(const zltBiTree *tree, int xy) {
  zltBiTree *parent = tree->parent;
  if (!parent) {
    return NULL;
  }
  if (tree == parent->children[xy]) {
    return parent;
  }
  return xny1(parent, xy);
}

zltBiTree *zltBiTreeXYN(const zltBiTree *tree, int xy) {
  zltBiTree *parent = tree->parent;
  if (!parent) {
    return NULL;
  }
  zltBiTree *y = parent->children[!xy];
  if (tree != y && y) {
    return zltBiTreeMostSide(y, xy);
  }
  return parent;
}
// iterators end

zltBiTree *zltBiTreeRotate(zltBiTree *tree, bool right) {
  zltBiTree *parent = tree->parent;
  zltBiTree *child = tree->children[!right];
  tree->parent = child;
  tree->children[!right] = child->children[right];
  child->parent = parent;
  child->children[right] = tree;
  if (parent) {
    parent->children[tree == parent->rchd] = child;
  }
  return child;
}

// find operations begin
zltBiTree *zltBiTreeFind(const zltBiTree *tree, zltBiTreeCmpForFind *cmp, const void *data) {
  if (!tree) {
    return NULL;
  }
  int diff = cmp(data, tree);
  if (!diff) {
    return (zltBiTree *) tree;
  }
  zltBiTree *next = tree->children[diff > 0];
  return zltBiTreeFind(next, cmp, data);
}

zltBiTree **zltBiTreeFindForInsert(zltBiTree **parent, zltBiTree **tree, zltBiTreeCmpForFind *cmp, const void *data) {
  if (!*tree) {
    return tree;
  }
  int diff = cmp(data, *tree);
  if (!diff) {
    return tree;
  }
  *parent = *tree;
  zltBiTree **next = &zltMemberOf(*tree, zltBiTree, children)[diff > 0];
  return zltBiTreeFindForInsert(parent, next, cmp, data);
}
// find operations end
