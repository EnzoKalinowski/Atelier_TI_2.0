#include "Movement.h"

void ** Vecteur (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch){
	int **tabaccu = imatrix0 (nrl, nrh, ncl, nch);
	int X = (nrl-nrh)/2;
	int Y = (ncl-nch)/2;
	int xplus =0;
	int yplus =0;
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if(imgT[i][j]==255){
				int vect = 1000;	
				for (int iplus= nrl; iplus< nrh; iplus++){
					for(int jplus= ncl; jplus<nch; jplus++){
						if(imgTplusUn[iplus][jplus]==255){
							int vectcalcule = abs(iplus-i)+(jplus-j);
							if (vectcalcule<vect){
								vect = vectcalcule;
								xplus = iplus;
								yplus = jplus;
							}
						}
					}
				}
			}
			tabaccu[i+xplus][j+yplus] ++; 
		}
	}
	int maxaccu = 0;
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if (tabaccu[i][j]>maxaccu){
				maxaccu = tabaccu[i][j];
				xplus = i;
				yplus = j;
			}
		}
	}
	x = &xplus;
	y = &yplus;
	free_imatrix (tabaccu, nrl, nrh, ncl, nch);
}