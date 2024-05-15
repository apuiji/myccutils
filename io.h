#ifndef ZLT_IO_H
#define ZLT_IO_H

#include<stdio.h>
#include"string.h"

#ifdef __cplusplus

extern "C" {

#endif

/// ignore bad
void zltFputs(FILE *dest, zltString s);

#ifdef __cplusplus

}

#endif

#endif
