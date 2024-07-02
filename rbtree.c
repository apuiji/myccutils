#include"zlt/rbtree.h"
#include"zlt/xyz.h"

void zltRBTreeSwap(zltBiTree **root, zltRBTree *a, zltRBTree *b) {
  zltBiTreeSwap(root, (zltBiTree *) a, (zltBiTree *) b);
  bool red = a->red;
  a->red = b->red;
  b->red = red;
}

// after insert operations begin
static void afterInsert1(zltBiTree **root, zltRBTree *node, zltRBTree *parent, zltRBTree *gparent);
static void afterInsert2(zltBiTree **root, zltRBTree *node, zltRBTree *parent, zltRBTree *gparent);

void zltRBTreeAfterInsert(zltBiTree **root, zltRBTree *node) {
  zltRBTree *parent = (zltRBTree *) node->biTree.parent;
  if (!parent) {
    node->red = false;
    *root = (zltBiTree *) node;
    return;
  }
  if (!parent->red) {
    return;
  }
  zltRBTree *gparent = (zltRBTree *) parent->biTree.parent;
  zltRBTree *uncle = (zltRBTree *) gparent->biTree.children[parent == zltBiTreeMemb(gparent, lchd)];
  if (uncle && zltRBTreeMemb(uncle, red)) {
    zltRBTreeMemb(parent, red) = false;
    zltRBTreeMemb(uncle, red) = false;
    zltRBTreeMemb(gparent, red) = true;
    zltRBTreeAfterInsert(root, gparent);
    return;
  }
  bool b = (node == zltBiTreeMemb(parent, lchd)) == (parent == zltBiTreeMemb(gparent, lchd));
  (b ? afterInsert1 : afterInsert2)(root, node, parent, gparent);
}

//     GB        PR        PB
//    /  \      /  \      /  \_
//   PR  UB -> NR  GB -> NR  GR
//  /  \          /  \      /  \_
// NR  SB        SB  UB    SB  UB
void afterInsert1(void **root, void *node, void *parent, void *gparent) {
  int left = node == zltBiTreeMemb(parent, lchd);
  zltBiTreeRotate(gparent, !left);
  zltRBTreeMemb(parent, red) = false;
  zltRBTreeMemb(gparent, red) = true;
  if (*root == gparent) {
    *root = parent;
  }
}

//   GB        GB        __NR__        __NB__
//  /  \      /  \      /      \      /      \_
// PR  UB -> NR  UB -> PR      GB -> PR      GR
//   \      /  \         \    /  \     \    /  \_
//   NR    PR  YB        XB  YB  UB    XB  YB  UB
//  /  \     \_
// XB  YB    XB
void afterInsert2(void **root, void *node, void *parent, void *gparent) {
  int left = node == zltBiTreeMemb(parent, lchd);
  zltBiTreeRotate(parent, !left);
  zltBiTreeRotate(gparent, left);
  zltRBTreeMemb(node, red) = false;
  zltRBTreeMemb(gparent, red) = true;
  if (*root == gparent) {
    *root = node;
  }
}
// after insert operations end

// before erase operations begin
static void beforeErase1(void **root, void *node);
static void beforeErase3(void **root, void *node);

void zltRBTreeBeforeErase(void **root, void *node) {
  beforeErase1(root, node);
  if (zltRBTreeMemb(node, red) || zltBiTreeMemb(node, lchd) || zltBiTreeMemb(node, rchd)) {
    return;
  }
  beforeErase3(root, node);
}

void beforeErase1(void **root, void *node) {
  void *a = zltBiTreeMemb(node, rchd);
  if (!a) {
    return;
  }
  a = zltBiTreeMostSide(a, 0);
  zltRBTreeSwap(root, node, a);
}

static void beforeErase4(void **root, void *node, void *sibling, void *parent);
static void beforeErase5(void **root, void *node, void *sibling, void *parent);

void beforeErase3(void **root, void *node) {
  void *parent = zltBiTreeMemb(node, parent);
  void *sibling = zltBiTreeMemb(parent, children)[node == zltBiTreeMemb(parent, lchd)];
  (sibling && zltRBTreeMemb(sibling, red) ? beforeErase4 : beforeErase5)(root, node, sibling, parent);
}

//   PB         SR        SB
//  /  \       /  \      /  \_
// NB  SR  -> PB  YB -> PR  YB
//    /  \   /  \      /  \_
//   XB  YB NB  XB    NB  XB
void beforeErase4(void **root, void *node, void *sibling, void *parent) {
  bool right = node == zltBiTreeMemb(parent, rchd);
  void *x = zltBiTreeMemb(sibling, children)[right];
  zltBiTreeRotate(parent, right);
  zltRBTreeMemb(sibling, red) = false;
  zltRBTreeMemb(parent, red) = true;
  if (*root == parent) {
    *root = sibling;
  }
  beforeErase5(root, node, x, parent);
}

static void beforeErase6(void **root, void *node, void *sibling, void *parent);
static void beforeErase7(void **root, void *node, void *sibling, void *parent);
static void beforeErase8(void **root, void *node, void *sibling, void *parent);

void beforeErase5(void **root, void *node, void *sibling, void *parent) {
  void (*f)(void **root, void *node, void *sibling, void *parent);
  bool right = node == zltBiTreeMemb(parent, rchd);
  void *x = zltBiTreeMemb(sibling, children)[right];
  if (x && zltRBTreeMemb(x, red)) {
    f = beforeErase6;
    goto A;
  }
  void *y = zltBiTreeMemb(sibling, children)[!right];
  if (y && zltRBTreeMemb(y, red)) {
    f = beforeErase7;
    goto A;
  }
  f = beforeErase8;
  A:
  f(root, node, sibling, parent);
}

//   PC        PC         __XR__        __XC__
//  /  \      /  \       /      \      /      \_
// NB  SB -> NB  XR  -> PC      SB -> PB      SB
//    /         /  \   /  \    /     /  \    /
//   XR        UB  SB NB  UB  VB    NB  UB  VB
//  /  \          /
// UB  VB        VB
void beforeErase6(void **root, void *node, void *sibling, void *parent) {
  bool right = node == zltBiTreeMemb(parent, rchd);
  void *x = zltBiTreeMemb(sibling, children)[right];
  zltBiTreeRotate(sibling, !right);
  zltBiTreeRotate(parent, right);
  zltRBTreeMemb(x, red) = zltRBTreeMemb(parent, red);
  zltRBTreeMemb(parent, red) = false;
  if (*root == parent) {
    *root = x;
  }
}

//   PC         SB        SC
//  /  \       /  \      /  \_
// NB  SB  -> PC  YR -> PB  YB
//    /  \   /  \      /  \_
//   XB  YR NB  XB    NB  XB
void beforeErase7(void **root, void *node, void *sibling, void *parent) {
  bool right = node == zltBiTreeMemb(parent, rchd);
  void *y = zltBiTreeMemb(sibling, children)[!right];
  zltBiTreeRotate(parent, right);
  zltRBTreeMemb(sibling, red) = zltRBTreeMemb(parent, red);
  zltRBTreeMemb(y, red) = false;
  zltRBTreeMemb(parent, red) = false;
  if (*root == parent) {
    *root = sibling;
  }
}

//   PC        PC        PB
//  /  \      /  \      /  \_
// NB  SB -> NB  SR -> NB  SR
//    /  \      /  \      /  \_
//   XB  YB    XB  YB    XB  YB
void beforeErase8(void **root, void *node, void *sibling, void *parent) {
  zltRBTreeMemb(sibling, red) = true;
  if (zltRBTreeMemb(parent, red)) {
    zltRBTreeMemb(parent, red) = false;
  } else {
    beforeErase3(root, parent);
  }
}
// before erase operations end

void zltRBTreeErase(void **root, void *node) {
  void *parent = zltBiTreeMemb(node, parent);
  void *child = zltBiTreeMemb(node, lchd);
  if (!child) {
    child = zltBiTreeMemb(node, rchd);
  }
  if (parent) {
    zltBiTreeMemb(parent, children)[node == zltBiTreeMemb(parent, rchd)] = child;
  } else {
    *root = child;
  }
  if (child) {
    zltBiTreeMemb(child, parent) = parent;
    zltRBTreeMemb(child, red) = false;
  }
}

void *zltRBTreeFindAndErase(void **root, zltBiTreeCmpForFind *cmp, const void *data) {
  void *node = zltBiTreeFind(*root, cmp, data);
  if (!node) {
    return NULL;
  }
  zltRBTreeBeforeErase(root, node);
  zltRBTreeErase(root, node);
  return node;
}
