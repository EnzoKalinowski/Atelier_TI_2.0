#include <stdio.h>
#include "Preprocessing.h"
#include "Tracking.h"


int main(){
    
    long nrh,nrl,nch,ncl;
	rgb8 **I;

	int tab[2];
	int x, y;
	I=LoadPPM_rgb8matrix("droite_600x450.ppm",&nrl,&nrh,&ncl,&nch);
	// getRedBarycenter(I, nrl, nrh, ncl, nch, tab);
	getBarycenterColor(I, nrl, nrh, ncl, nch, &x, &y);
    printf("%d, %d\n", x, y);

    return 0;
}