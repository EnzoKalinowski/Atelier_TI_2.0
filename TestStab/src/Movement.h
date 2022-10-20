#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "./NRC/def.h"
#include "./NRC/nrio.h"
#include "./NRC/nrarith.h"
#include "./NRC/nralloc.h"

typedef struct Element Element;
struct Element
{
    int i;
    int j;
    Element *suivant;
};

typedef struct Liste Liste;
struct Liste
{
    Element *premier;
};

void ** Vecteur (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch);
void ** VecteurOpti (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch);
Liste *initialisation();
void insertion(Liste *liste, int i, int j);
void suppression(Liste *liste);

#endif // __MOVEMENT_H__