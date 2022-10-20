#include "Movement.h"

void ** Vecteur (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch){
	int **tabaccu = imatrix0 (nrl, nrh, ncl, nch);
	int X = (nrl-nrh)/2;
	int Y = (ncl-nch)/2;
	int xplus =0;
	int yplus =0;
	//printf("avant la lecture des image et l'insertion dans la table d'accumulation\n");
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if(imgT[i][j]==255){
				//printf("1ER IF\n");
				int vect = 100000;	
				for (int iplus= nrl; iplus< nrh; iplus++){
					for(int jplus= ncl; jplus<nch; jplus++){
						if(imgTplusUn[iplus][jplus]==255){
							//printf("2ND IF\n");
							//printf("iplus = %d/%d, jplus = %d/%d \n",iplus,nrh,jplus,nch);
							int vectcalcule = abs(iplus-i)+(jplus-j);
							//printf("APRES VECTCALCULE!!!\n");
							if (vectcalcule<vect){
								//printf("3EME IF\n");
								vect = vectcalcule;
								xplus = iplus;
								yplus = jplus;
							}
						}
					}
				}
			//printf("Avant de remplir tabaccu\n");
			tabaccu[xplus][yplus] ++; 
			//printf("Apres avoir rempli tabaccu\n");
			}
		}
	}
	//printf("apres avoir rempli la table d'accumulation\n");
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

void ** VecteurOpti (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch){
	int **tabaccu = imatrix0 (nrl, nrh, ncl, nch);
	int X = (nrl-nrh)/2;
	int Y = (ncl-nch)/2;
	int xplus =0;
	int yplus =0;
	//printf("DEBUT\n");
	Liste *listePointsInterets = initialisation();
	//printf("listePointsInterets CREE\n");

	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			//printf("AVANT LE IF, 	i = %d, j = %d \n",i,j);
			if(imgTplusUn[i][j]==255){
				//prin("DANS LE IF AVANT L'AJOUT\n");
				insertion(listePointsInterets, i, j);
				//printf("DANS LE IF APRES L'AJOUT\n");
			}
			//printf("APRES LE IF\n");
		}
	}
	//printf("PREMIER FOR PASSE, on a techniquement ajoute tt les points d interets\n");
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if(imgT[i][j]==255){
				int vect = 100000;
				Element *actuel = listePointsInterets->premier;
				while(actuel!=NULL){
					int vectcalcule = abs(actuel->i-i)+(actuel->j-j);
					if (vectcalcule<vect){
						vect = vectcalcule;
						xplus = actuel->i;
						yplus = actuel->j;
					}
					actuel = actuel->suivant;
				}
				tabaccu[xplus][yplus] ++; 
			}
		}
	}
	//printf("DEUXIEME FOR PASSE, on a parcouru tt la seconde image \n");
	long maxaccu = 0;
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if (tabaccu[i][j]>maxaccu){
				maxaccu = tabaccu[i][j];
				xplus = i;
				yplus = j;
			}
			////printf("%d",tabaccu[i][j]);
		}
		////printf("\n");
	}
	//printf("TROISIEME FORT PASSE\n");
	////printf("apres avoir recupere le vecteur\n");
	////printf("xplus = %d, yplus = %d\n",xplus,yplus);
	*x = xplus;
	*y = yplus;
	////printf("x = %d, y = %d\n",x,y);
	////printf("fin de la fonction \n");
	free_imatrix (tabaccu, nrl, nrh, ncl, nch);
	//printf("On supp la liste\n");
	suppression(listePointsInterets);
}


Liste *initialisation()
{
    //Liste *liste = malloc(sizeof(*liste));
	Liste *liste = new Liste;
    //Element *element = malloc(sizeof(*element));
	Element *element = new Element;

    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element->i = 0;
	element->j = 0;
    element->suivant = NULL;
    liste->premier = element;

    return liste;
}

void insertion(Liste *liste, int i, int j)
{
    /* Création du nouvel élément */
    //Element *nouveau = malloc(sizeof(*nouveau));
	Element *nouveau = new Element;
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    nouveau->i = i;
	nouveau->j = j;

    /* Insertion de l'élément au début de la liste */
    nouveau->suivant = liste->premier;
    liste->premier = nouveau;
}

void suppression(Liste *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste->premier != NULL)
    {
        Element *aSupprimer = liste->premier;
        liste->premier = liste->premier->suivant;
        free(aSupprimer);
    }
}