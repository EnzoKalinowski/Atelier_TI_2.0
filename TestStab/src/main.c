#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Movement.h"
#include "Tracking.h"

int main(){

    time_t temps1 = time(NULL);
    int x, y =0;
    float sigma = 0.5;
    int size = 3;
    long nrl, nrh, ncl, nch;
    //printf("On cree le filtre\n");
    double** filter = create_gaussian_filter(sigma, size);
    /*
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            printf("%f ",filter[i][j]);
        }
        printf("\n");
    }
    */
    //on ouvre les deux images;
    //printf("On ouvre les deux Images\n");
    //printf("On ouvre la premiere Image\n");
    rgb8 **ImgT = LoadPPM_rgb8matrix("../droite.ppm", &nrl, &nrh, &ncl, &nch);
    //printf("On a ouvert la premiere img\n");
    //printf("On ouvre la seconde Image\n");
    rgb8 **ImgTPlusUn = LoadPPM_rgb8matrix("../gauche.ppm", &nrl, &nrh, &ncl, &nch);
    //printf("On a ouvert la seconde img\n");
    byte **ImgTGris;
    ImgTGris=bmatrix(nrl, nrh, ncl, nch);
    
    //printf("On convertis la premire image en nuance de gris \n");
    //convert_rgb8_to_byte(rgb8 **I, byte **B, long nrl, long nrh, long ncl, long nch);
    convert_rgb8_to_byte(ImgT, ImgTGris, nrl, nrh, ncl, nch);
    SavePGM_bmatrix(ImgTGris,nrl, nrh, ncl, nch,"ImgTGris.pgm");
    byte **ImgTPlusUnGris;
    ImgTPlusUnGris=bmatrix(nrl, nrh, ncl, nch);
    
    
    //printf("On convertis la seconde image en nuance de gris \n");
    convert_rgb8_to_byte(ImgTPlusUn, ImgTPlusUnGris, nrl, nrh, ncl, nch);
    SavePGM_bmatrix(ImgTGris,nrl, nrh, ncl, nch,"ImgTPlusUnGris.pgm");

    int filter_size = size;
    float lambda = sigma;
    
    //printf("On appel Harris sur la premiere Img \n");
    double ** ImgTHarris = harris(ImgTGris, filter, filter_size, lambda, nrl, nrh, ncl, nch);
    //printf("On appel Harris sur la seconde Img \n");
    double ** ImgTPlusUnHarris =harris(ImgTPlusUnGris, filter, filter_size, lambda, nrl, nrh, ncl, nch);

    //void convert_dmatrix_bmatrix(double **D, byte **B, long nrl, long nrh, long ncl, long nch)
    //printf("On converti la premiere Img \n");
    convert_dmatrix_bmatrix(ImgTHarris, ImgTGris, nrl, nrh, ncl, nch);
    SavePGM_bmatrix(ImgTGris,nrl, nrh, ncl, nch,"ImgTHarris.pgm");
    //printf("On converti la seconde Img \n");
    convert_dmatrix_bmatrix(ImgTPlusUnHarris, ImgTPlusUnGris, nrl, nrh, ncl, nch);
    SavePGM_bmatrix(ImgTPlusUnGris,nrl, nrh, ncl, nch,"ImgTPlusUnHarris.pgm");


    //void binarize(byte **I, byte **B, int treshold, long nrl, long nrh, long ncl, long nch)


    //Vecteur (double **imgT, double **imgTplusUn, int *x, int *y, long nrl, long nrh, long ncl, long nch)
    //printf("On calcul le vecteur \n");
    Vecteur (ImgTGris, ImgTPlusUnGris, &x, &y, nrl, nrh, ncl, nch);
    //printf("x = %p, y = %p.",*x,*y);
    printf("x = %d, y = %d.\n",x,y);
    //printf("FIN !!! \n");

    time_t temps2 = time(NULL);
    float time = temps2 - temps1;
    printf("Le progrqme a prit %f secondes. \n", time);

    free_rgb8matrix(ImgT, nrl, nrh, ncl, nch);
    free_rgb8matrix(ImgTPlusUn, nrl, nrh, ncl, nch);


    free_bmatrix  (ImgTGris, nrl, nrh, ncl, nch);
    free_bmatrix  (ImgTPlusUnGris, nrl, nrh, ncl, nch);

    free_dmatrix(ImgTHarris, nrl, nrh, ncl, nch);
    free_dmatrix(ImgTPlusUnHarris, nrl, nrh, ncl, nch);

    return 0;
}