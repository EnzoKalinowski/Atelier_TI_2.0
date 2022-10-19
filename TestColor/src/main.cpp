#include <stdio.h>
#include "Preprocessing.h"
#include "Tracking.h"


int main(){
    
    long nrh,nrl,nch,ncl;
	rgb8 **I;

	int tab[2];
	I=LoadPPM_rgb8matrix("droite.ppm",&nrl,&nrh,&ncl,&nch);
	getRedBarycenter(I, nrl, nrh, ncl, nch, tab);
    printf("%d, %d", tab, (tab+1));

    return 0;
}