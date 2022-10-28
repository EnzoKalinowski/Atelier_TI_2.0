#include "Movement.h"


// premiere fonction de calcul du vecteur de maniere non optimisee
void ** Vecteur (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch){
	int **tabaccu = imatrix0 (nrl, nrh, ncl, nch); // Creqtion de la table 'accumulation
	int X = (nrh-nrl)/2;
	int Y = (nch-ncl)/2;
	int xplus =0;
	int yplus =0;
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){ // Parcourt de l'image T
			if(imgT[i][j]>200){ // seuil definissant un point d'interet 
				int vect = 100000;	
				for (int iplus= nrl; iplus< nrh; iplus++){
					for(int jplus= ncl; jplus<nch; jplus++){ // Parcourt de la seconde image 
						if(imgTplusUn[iplus][jplus]>200){ // seuil definissant un point d'interet 
							int vectcalcule = abs(iplus-i)+(jplus-j); // calcul de la distance entre les deux points 
							if (vectcalcule<vect){ // mise a jour du point associe au vecteur optimal
								vect = vectcalcule;
								xplus = iplus-i;
								yplus = jplus-j;
							}
						}
					}
				}
			tabaccu[xplus][yplus] ++; // incrementation de la table d'accumulation
			}
		}
	}
	long maxaccu = 0;
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){ // parcourt de la table d'accumulation
			if (tabaccu[i][j]>maxaccu){// mise a jour du vecteur optimal 
				maxaccu = tabaccu[i][j];
				xplus = i;
				yplus = j;
			}
		}
	}
	*x = xplus; // retour des coordonnees du point associe au vectuer optimal
	*y = yplus;
	free_imatrix (tabaccu, nrl, nrh, ncl, nch); // liberation de la table d'accumulation
}

// seconde fonction de calcul du vecteur de maniere  optimisee
void ** VecteurOpti (byte **imgT, byte **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch){
	int **tabaccu = imatrix0 (nrl, nrh, ncl, nch); // Creqtion de la table 'accumulation
	int X = (nrh-nrl)/2;
	int Y = (nch-ncl)/2;
	int xplus =0;
	int yplus =0;
	Liste *listePointsInterets = initialisation(); // creation de 4 listes de points 
	Liste *listePointsInterets2 = initialisation();
	Liste *listePointsInterets3 = initialisation();
	Liste *listePointsInterets4 = initialisation();
	int nrhsurdeux = X;
	int nchsurdeux = Y;
	std::thread t1(listePT,listePointsInterets,imgTplusUn,nrl,nrhsurdeux,ncl,nchsurdeux); // multithreading en 4 du parcourt et de l'ajout dans les 4 listes des points d'interets de l'image T+1
	std::thread t2(listePT,listePointsInterets2,imgTplusUn,nrl,nrhsurdeux,nchsurdeux,nch);
	std::thread t3(listePT,listePointsInterets3,imgTplusUn,nrhsurdeux,nrh,ncl,nchsurdeux);
	std::thread t4(listePT,listePointsInterets4,imgTplusUn,nrhsurdeux,nrh,nchsurdeux,nch);
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	/*for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){ // Pqrcourt de l'image T+1
			if(imgTplusUn[i][j]>200){// seuil definissant un point d'interet 
				insertion(listePointsInterets, i, j);
			}
		}
	}*/
	std::thread t5(collerListes,listePointsInterets3,listePointsInterets4); //multithreading du regroupement des 4 listes en 1 liste
	std::thread t6(collerListes,listePointsInterets2,listePointsInterets3);
	std::thread t7(collerListes,listePointsInterets,listePointsInterets2);
	t5.join();
	t6.join();
	t7.join();
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){ // Parcourt de l'image T
			if(imgT[i][j]>200){// seuil definissant un point d'interet 
				int vect = 100000;
				Element *actuel = listePointsInterets->premier; 
				while(actuel!=NULL){// pour chaque point d'interet de T on parcourt la liste chainee des points de T+1
					int vectcalcule = abs(actuel->i-i)+(actuel->j-j); // calcul de la distance entre les deux points 
					if (vectcalcule<vect){// mise a jour du point associe au vecteur optimal
						vect = vectcalcule;
						xplus = actuel->i-i;
						yplus = actuel->j-j;
					}
					actuel = actuel->suivant;
				}
				tabaccu[X+xplus][Y+yplus] ++; // incrementation de la table d'accumulation
			}
		}
	}
	long maxaccu = 0;
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){// parcourt de la table d'accumulation
			if (tabaccu[i][j]>maxaccu){// mise a jour du vecteur optimal 
				maxaccu = tabaccu[i][j];
				xplus = i;
				yplus = j;// retour des coordonnees du point associe au vectuer optimal
			}
		}
	}
	*x = xplus-X;
	*y = yplus-Y;// retour des coordonnees du point associe au vectuer optimal
	free_imatrix (tabaccu, nrl, nrh, ncl, nch); // liberation de la table d'accumulation
	std::thread t8(suppression,listePointsInterets2); // liberation des listes chainees 
	std::thread t9(suppression,listePointsInterets3);
	std::thread t10(suppression,listePointsInterets4);
	std::thread t11(suppression,listePointsInterets);
	t8.join();
	t9.join();
	t10.join();
	t11.join();
}

// Fonction de parcourt d4une image et d'ajout de ces points d'interets dans une liste chainee 
void listePT(Liste *listePointsInterets,byte **imgTplusUn,long nrl, long nrh, long ncl, long nch){
	for (int i= nrl; i< nrh; i++){
		for(int j= ncl; j<nch; j++){ // Parcourt de l'image 
			if(imgTplusUn[i][j]==255){ // seuil definissant un point d'interet 
				insertion(listePointsInterets, i, j); // ajout dans la liste des coordonnees du point 
			}
		}
	}	
}

Liste *initialisation()
{
	Liste *liste = new Liste;
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
	Element *nouveau = new Element;
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    nouveau->i = i;
	nouveau->j = j;

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