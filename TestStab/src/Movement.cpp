#include "Movement.h"

void ** Vecteur (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch){
	int **tabaccu = imatrix0 (nrl, nrh, ncl, nch);
	int X = (nrh-nrl)/2;
	int Y = (nch-ncl)/2;
	int xplus =0;
	int yplus =0;
	//printf("avant la lecture des image et l'insertion dans la table d'accumulation\n");
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if(imgT[i][j]>200){
				//printf("1ER IF\n");
				int vect = 100000;	
				for (int iplus= nrl; iplus< nrh; iplus++){
					for(int jplus= ncl; jplus<nch; jplus++){
						if(imgTplusUn[iplus][jplus]>200){
							//printf("2ND IF\n");
							//printf("iplus = %d/%d, jplus = %d/%d \n",iplus,nrh,jplus,nch);
							int vectcalcule = abs(iplus-i)+(jplus-j);
							//printf("APRES VECTCALCULE!!!\n");
							if (vectcalcule<vect){
								//printf("3EME IF\n");
								vect = vectcalcule;
								xplus = iplus-i;
								yplus = jplus-j;
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
	int X = (nrh-nrl)/2;
	int Y = (nch-ncl)/2;
	int xplus =0;
	int yplus =0;
	//printf("DEBUT\n");
	Liste *listePointsInterets = initialisation();
	Liste *listePointsInterets2 = initialisation();
	Liste *listePointsInterets3 = initialisation();
	Liste *listePointsInterets4 = initialisation();
	//printf("listePointsInterets CREE\n");
	int nrhsurdeux = X;
	int nchsurdeux = Y;
	std::thread t1(listePT,listePointsInterets,imgTplusUn,nrl,nrhsurdeux,ncl,nchsurdeux);
	std::thread t2(listePT,listePointsInterets2,imgTplusUn,nrl,nrhsurdeux,nchsurdeux,nch);
	std::thread t3(listePT,listePointsInterets3,imgTplusUn,nrhsurdeux,nrh,ncl,nchsurdeux);
	std::thread t4(listePT,listePointsInterets4,imgTplusUn,nrhsurdeux,nrh,nchsurdeux,nch);
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	//printf("TEST\n");
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			//printf("AVANT LE IF, 	i = %d, j = %d \n",i,j);
			if(imgTplusUn[i][j]>200){
				//prin("DANS LE IF AVANT L'AJOUT\n");
				insertion(listePointsInterets, i, j);
				//printf("DANS LE IF APRES L'AJOUT\n");
			}
			//printf("APRES LE IF\n");
		}
	}
	std::thread t5(collerListes,listePointsInterets3,listePointsInterets4);
	std::thread t6(collerListes,listePointsInterets2,listePointsInterets3);
	std::thread t7(collerListes,listePointsInterets,listePointsInterets2);
	t5.join();
	t6.join();
	t7.join();
	//printf("PREMIER FOR PASSE, on a techniquement ajoute tt les points d interets\n");
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){
			if(imgT[i][j]>200){
				int vect = 100000;
				Element *actuel = listePointsInterets->premier;
				while(actuel!=NULL){
					//printf("AVANT le if du remplacement du meilleur vect\n");
					int vectcalcule = abs(actuel->i-i)+(actuel->j-j);
					if (vectcalcule<vect){
						//printf("dans le if du remplacement du meilleur vect\n");
						vect = vectcalcule;
						xplus = actuel->i-i;
						yplus = actuel->j-j;
					}
					actuel = actuel->suivant;
				}
				tabaccu[X+xplus][Y+yplus] ++; 
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
	*x = xplus-X;
	*y = yplus-Y;
	////printf("x = %d, y = %d\n",x,y);
	////printf("fin de la fonction \n");
	free_imatrix (tabaccu, nrl, nrh, ncl, nch);
	//printf("On supp la liste\n");
	//suppression(listePointsInterets);
	std::thread t8(suppression,listePointsInterets2);
	std::thread t9(suppression,listePointsInterets3);
	std::thread t10(suppression,listePointsInterets4);
	std::thread t11(suppression,listePointsInterets);
	t8.join();
	t9.join();
	t10.join();
	t11.join();
}

void listePT(Liste *listePointsInterets,byte **imgTplusUn,long nrl, long nrh, long ncl, long nch){
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

void collerListes(Liste *liste1, Liste *liste2){
	Element *actuel = liste1->premier;
	while(actuel->suivant!=NULL){
		actuel = actuel->suivant;
	}
	actuel->suivant = liste2->premier;
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