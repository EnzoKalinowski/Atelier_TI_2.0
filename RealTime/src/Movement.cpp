#include "Movement.h"

void ** Vecteur (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch){
	
	int **tabaccu = imatrix0(nrl, nrh, ncl, nch);
	int X = (nrl-nrh)/2;
	int Y = (ncl-nch)/2;
	int xplus =0;
	int yplus =0;
	// printf("avant la lecture des image et l'insertion dans la table d'accumulation\n");
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if(imgT[i][j]==255){
				// printf("1ER IF\n");
				int vect = 100000;	
				for (int iplus= nrl; iplus< nrh; iplus++){
					for(int jplus= ncl; jplus<nch; jplus++){
						if(imgTplusUn[iplus][jplus]==255){
							// printf("2ND IF\n");
							// printf("iplus = %d/%d, jplus = %d/%d \n",iplus,nrh,jplus,nch);
							int vectcalcule = abs(iplus-i)+(jplus-j);
							// printf("APRES VECTCALCULE!!!\n");
							if (vectcalcule<vect){
								// printf("3EME IF\n");
								vect = vectcalcule;
								xplus = iplus;
								yplus = jplus;
							}
						}
					}
				}
			// printf("Avant de remplir tabaccu\n");
			tabaccu[xplus][yplus] ++; 
			// printf("Apres avoir rempli tabaccu\n");
			}
		}
	}
	// printf("apres avoir rempli la table d'accumulation\n");
	long maxaccu = 0;
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if (tabaccu[i][j]>maxaccu){
				maxaccu = tabaccu[i][j];
				xplus = i;
				yplus = j;
			}
			// printf("%d",tabaccu[i][j]);
		}
		// printf("\n");
	}
	// printf("apres avoir recupere le vecteur\n");
	// printf("xplus = %d, yplus = %d\n",xplus,yplus);
	*x = xplus;
	*y = yplus;
	// printf("x = %d, y = %d\n",x,y);
	// printf("fin de la fonction \n");
	free_imatrix (tabaccu, nrl, nrh, ncl, nch);
}
/*
void ** VecteurOpti (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch){
	int **tabaccu = imatrix0 (nrl, nrh, ncl, nch);
	int X = (nrl-nrh)/2;
	int Y = (ncl-nch)/2;
	int xplus =0;
	int yplus =0;
	
	Liste *listePointsInterets;
	nouvListe(listePointsInterets);


	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if(imgTplusUn[i][j]==255){
				ajoutListe(listePointsInterets, i, j);
			}
		}
	}
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if(imgT[i][j]==255){
				int vect = 100000;
				while(listePointsInterets->suivant!=NULL){
					int vectcalcule = abs(listePointsInterets->i-i)+(listePointsInterets->j-j);
					if (vectcalcule<vect){
						vect = vectcalcule;
						xplus = listePointsInterets->i;
						yplus = listePointsInterets->j;
					}
				}
				tabaccu[xplus][yplus] ++; 
			}
		}
	}
	long maxaccu = 0;
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if (tabaccu[i][j]>maxaccu){
				maxaccu = tabaccu[i][j];
				xplus = i;
				yplus = j;
			}
			//printf("%d",tabaccu[i][j]);
		}
		//printf("\n");
	}
	//printf("apres avoir recupere le vecteur\n");
	//printf("xplus = %d, yplus = %d\n",xplus,yplus);
	*x = xplus;
	*y = yplus;
	//printf("x = %d, y = %d\n",x,y);
	//printf("fin de la fonction \n");
	free_imatrix (tabaccu, nrl, nrh, ncl, nch);
}


int estVide(Liste *liste){
	if(*liste->i==NULL){
		return 1;
	}
	return 0;
}

Liste nouvListe(Liste *liste){
	liste = malloc(sizeof(*liste));
	liste->i=NULL;
	liste->j=NULL;
	liste->suivant=NULL;
	return *liste;
}

Liste ajoutListe(Liste *liste, int i, int j){
	Liste *nouvelem;
	nouvelem = malloc(sizeof(*nouvelem));
	nouvelem->i = i;
	nouvelem->j = j;
	if(estVide(liste)==1){
		nouvelem->suivant=NULL;
	}
	else{
		nouvelem->suivant = liste;
	}
}*/