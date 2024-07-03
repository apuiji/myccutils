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
  zltRBTree *uncle = (zltRBTree *) gparent->biTree.children[parent == (void *) gparent->biTree.lchd];
  if (uncle && uncle->red) {
    parent->red = false;
    uncle->red = false;
    gparent->red = true;
    zltRBTreeAfterInsert(root, gparent);
    return;
  }
  bool b = (node == (void *) parent->biTree.lchd) == (parent == (void *) gparent->biTree.lchd);
  (b ? afterInsert1 : afterInsert2)(root, node, parent, gparent);
}

//     GB        PR        PB
//    /  \      /  \      /  \_
//   PR  UB -> NR  GB -> NR  GR
//  /  \          /  \      /  \_
// NR  SB        SB  UB    SB  UB
void afterInsert1(zltBiTree **root, zltRBTree *node, zltRBTree *parent, zltRBTree *gparent) {
  int left = node == (void *) parent->biTree.lchd;
  zltBiTreeRotate((zltBiTree *) gparent, !left);
  parent->red = false;
  gparent->red = true;
  if (*root == (void *) gparent) {
    *root = (zltBiTree *) parent;
  }
}

//   GB        GB        __NR__        __NB__
//  /  \      /  \      /      \      /      \_
// PR  UB -> NR  UB -> PR      GB -> PR      GR
//   \      /  \         \    /  \     \    /  \_
//   NR    PR  YB        XB  YB  UB    XB  YB  UB
//  /  \     \_
// XB  YB    XB
void afterInsert2(zltBiTree **root, zltRBTree *node, zltRBTree *parent, zltRBTree *gparent) {
  int left = node == (void *) parent->biTree.lchd;
  zltBiTreeRotate((zltBiTree *) parent, !left);
  zltBiTreeRotate((zltBiTree *) gparent, left);
  node->red = false;
  gparent->red = true;
  if (*root == (void *) gparent) {
    *root = (zltBiTree *) node;
  }
}
// after insert operations end

// before erase operations begin
static void beforeErase1(zltBiTree **root, zltRBTree *node);
static void beforeErase3(zltBiTree **root, zltRBTree *node);

void zltRBTreeBeforeErase(zltBiTree **root, zltRBTree *node) {
  beforeErase1(root, node);
  if (node->red || node->biTree.lchd || node->biTree.rchd) {
    return;
  }
  beforeErase3(root, node);
}

void beforeErase1(zltBiTree **root, zltRBTree *node) {
  zltRBTree *a = (zltRBTree *) node->biTree.rchd;
  if (!a) {
    return;
  }
  a = (zltRBTree *) zltBiTreeMostLeft((zltBiTree *) a);
  zltRBTreeSwap(root, node, a);
}

static void beforeErase4(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent);
static void beforeErase5(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent);

void beforeErase3(zltBiTree **root, zltRBTree *node) {
  zltRBTree *parent = (zltRBTree *) node->biTree.parent;
  zltRBTree *sibling = (zltRBTree *) parent->biTree.children[node == (void *) parent->biTree.lchd];
  (sibling && sibling->red ? beforeErase4 : beforeErase5)(root, node, sibling, parent);
}

//   PB         SR        SB
//  /  \       /  \      /  \_
// NB  SR  -> PB  YB -> PR  YB
//    /  \   /  \      /  \_
//   XB  YB NB  XB    NB  XB
void beforeErase4(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent) {
  bool right = node == (void *) parent->biTree.rchd;
  zltRBTree *x = (zltRBTree *) sibling->biTree.children[right];
  zltBiTreeRotate((zltBiTree *) parent, right);
  sibling->red = false;
  parent->red = true;
  if (*root == (void *) parent) {
    *root = (zltBiTree *) sibling;
  }
  beforeErase5(root, node, x, parent);
}

static void beforeErase6(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent);
static void beforeErase7(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent);
static void beforeErase8(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent);

void beforeErase5(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent) {
  void (*f)(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent);
  bool right = node == (void *) parent->biTree.rchd;
  zltRBTree *x = (zltRBTree *) sibling->biTree.children[right];
  if (x && x->red) {
    f = beforeErase6;
    goto A;
  }
  zltRBTree *y = (zltRBTree *) sibling->biTree.children[!right];
  if (y && y->red) {
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
void beforeErase6(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent) {
  bool right = node == (void *) parent->biTree.rchd;
  zltRBTree *x = (zltRBTree *) sibling->biTree.children[right];
  zltBiTreeRotate((zltBiTree *) sibling, !right);
  zltBiTreeRotate((zltBiTree *) parent, right);
  x->red = parent->red;
  parent->red = false;
  if (*root == (void *) parent) {
    *root = (zltBiTree *) x;
  }
}

//   PC         SB        SC
//  /  \       /  \      /  \_
// NB  SB  -> PC  YR -> PB  YB
//    /  \   /  \      /  \_
//   XB  YR NB  XB    NB  XB
void beforeErase7(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent) {
  bool right = node == (void *) parent->biTree.rchd;
  zltRBTree *y = (zltRBTree *) sibling->biTree.children[!right];
  zltBiTreeRotate((zltBiTree *) parent, right);
  sibling->red = parent->red;
  y->red = false;
  parent->red = false;
  if (*root == (void *) parent) {
    *root = (zltBiTree *) sibling;
  }
}

//   PC        PC        PB
//  /  \      /  \      /  \_
// NB  SB -> NB  SR -> NB  SR
//    /  \      /  \      /  \_
//   XB  YB    XB  YB    XB  YB
void beforeErase8(zltBiTree **root, zltRBTree *node, zltRBTree *sibling, zltRBTree *parent) {
  sibling->red = true;
  if (parent->red) {
    parent->red = false;
  } else {
    beforeErase3(root, parent);
  }
}
// before erase operations end

void zltRBTreeErase(zltBiTree **root, zltRBTree *node) {
  zltRBTree *parent = (zltRBTree *) node->biTree.parent;
  zltRBTree *child = (zltRBTree *) node->biTree.lchd;
  if (!child) {
    child = (zltRBTree *) node->biTree.rchd;
  }
  if (parent) {
    parent->biTree.children[node == (void *) parent->biTree.rchd] = (zltBiTree *) child;
  } else {
    *root = (zltBiTree *) child;
  }
  if (child) {
    child->biTree.parent = (zltBiTree *) parent;
    child->red = false;
  }
}

zltRBTree *zltRBTreeFindAndErase(zltBiTree **root, zltBiTreeCmpForFind *cmp, const void *data) {
  zltRBTree *node = (zltRBTree *) zltBiTreeFind(*root, cmp, data);
  if (!node) {
    return NULL;
  }
  zltRBTreeBeforeErase(root, node);
  zltRBTreeErase(root, node);
  return node;
}
