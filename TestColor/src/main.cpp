#include <stdio.h>
#include <stdlib.h>
#include "Movement.h"
#include "Tracking.h"
#include "Preprocessing.h"


int main(){
    
    long nrh,nrl,nch,ncl;
	rgb8 **I;
	byte **R;
	float sum=0;
	int i,j;
	int tab[2];
	I=LoadPPM_rgb8matrix("droite.ppm",&nrl,&nrh,&ncl,&nch);
	getRedBarycenter(I, nrl, nrh, ncl, nch, tab);
    printf("%d, %d", tab, (tab+1));

    return 0;
}