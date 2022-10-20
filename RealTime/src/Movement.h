#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "./NRC/def.h"
#include "./NRC/nrio.h"
#include "./NRC/nrarith.h"
#include "./NRC/nralloc.h"

typedef struct Liste Liste;
struct Liste{
    int *i;
    int *j;
    struct Liste *suivant;
};

void ** Vecteur (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch);
void ** VecteurOpti (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch);
Liste nouvListe(Liste *liste);
int estVide(Liste *liste);
Liste ajoutListe(Liste *liste, int i, int j);

#endif // __MOVEMENT_H__