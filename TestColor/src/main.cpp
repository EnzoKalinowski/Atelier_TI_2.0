#include <stdio.h>
#include <stdlib.h>
#include "Movement.h"
#include "Tracking.h"
#include "Preprocessing.h"


int main(){
    
    long nrh,nrl,nch,ncl;
	byte **I;
	byte **R;
	float sum=0;
	int i,j;
	int tab[2];
	I=LoadPPM_rgb8matrix("cubes.pgm",&nrl,&nrh,&ncl,&nch);
	tab = getRedBarycenter(I, nrl, nrh, ncl, nch);
    printf("%d, %d" &tab, &(tab+1));

    return 0;
}