#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "./NRC/def.h"
#include "./NRC/nrio.h"
#include "./NRC/nrarith.h"
#include "./NRC/nralloc.h"

void ** Vecteur (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch);

#endif // __MOVEMENT_H__