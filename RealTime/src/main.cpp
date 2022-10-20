#include <stdio.h>
#include <stdlib.h>
#include "Movement.h"
#include "Tracking.h"
#include "Preprocessing.h"
#include <sys/time.h>
#include "opencv2/opencv.hpp"

using namespace cv;

using namespace std;

#define STABILISATION_MODE

double diff_ms(timeval t1, timeval t2)
{
    return (((t1.tv_sec - t2.tv_sec) * 1000000) +
            (t1.tv_usec - t2.tv_usec))/1000;
}

int main(){
    //images
    Mat image;
    Mat imageResized;
    rgb8 ** rgbImage;
    rgb8 ** rgbImage2;
    byte **byteImage;
    byte **byteImage2;
    double ** harrisImage;
    double ** harrisImage2;
    long nrl,nrh,ncl,nch;

    char fpsDisplay[50];
    int x,y;
    FILE* fichier = NULL;
    timeval t1 ,t2;
    double ms;
    double fps;

    //gaussian filter parameters
    float sigma = 0.5;
    int size = 3;

    float lambda = sigma;


    double** filter = create_gaussian_filter(sigma, size);


    // fichier = fopen("/dev/ttyACM0", "w");
    
    // if (fichier != NULL)
    // {   printf("Communication série initialisée\n");
    //     fclose(fichier);
    // }else
    // {
    //     perror("\t/!\\ PORT SERIE INTROUVABLE \n");
    // }

    namedWindow("Camera");
 
    VideoCapture cap(2);

    if (!cap.isOpened())
    {
        perror("\t/!\\ CANNOT OPEN CAMERA\n");
    }
    cap.read(image);
    resize(image,imageResized,Size(896,504));

    rgbImage=convertMatToRGB8(imageResized, &nrl, &nrh, &ncl, &nch);
    byteImage=bmatrix(nrl, nrh, ncl, nch);
    byteImage2=bmatrix(nrl, nrh, ncl, nch);


    convert_rgb8_to_byte(rgbImage, byteImage, nrl, nrh, ncl, nch);
    harrisImage = harris(byteImage, filter, size, lambda, nrl, nrh, ncl, nch);
    convert_dmatrix_bmatrix(harrisImage, byteImage, nrl, nrh, ncl, nch);

    while (true)
    {
        gettimeofday(&t1,0);
        
        
        cap.read(image);
        resize(image,imageResized,Size(896,504));
        
        // DEBUT TRAITEMENT

        rgbImage2=convertMatToRGB8(imageResized, &nrl, &nrh, &ncl, &nch);
        convert_rgb8_to_byte(rgbImage2, byteImage2, nrl, nrh, ncl, nch);
        harrisImage2 = harris(byteImage2, filter, size, lambda, nrl, nrh, ncl, nch);
        convert_dmatrix_bmatrix(harrisImage2, byteImage2, nrl, nrh, ncl, nch);


        Vecteur (byteImage, byteImage2, &x, &y, nrl, nrh, ncl, nch);
        // printf("x = %d, y = %d.\n",x,y);

        byteImage=byteImage2;

        //FIN TRAITEMENT 

        gettimeofday(&t2,0);
        ms=diff_ms(t2,t1);
        fps=1.0/(ms/1000);

        // sprintf (name, "FPS : %f", 1/((double)ms/1000));
        sprintf (fpsDisplay, "FPS : %f     x: %d y:%d", fps,x,y);

        putText(image, fpsDisplay, cv::Point(30,30), FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(20,20,25), 1, LINE_AA);
        imshow("Camera", image);

        if(waitKey(33) == 27) break;
    }

        free_rgb8matrix(rgbImage, nrl, nrh, ncl, nch);
        free_rgb8matrix(rgbImage2, nrl, nrh, ncl, nch);

        free_bmatrix  (byteImage, nrl, nrh, ncl, nch);
        free_bmatrix  (byteImage2, nrl, nrh, ncl, nch);

        free_dmatrix(harrisImage, nrl, nrh, ncl, nch);
        free_dmatrix(harrisImage2, nrl, nrh, ncl, nch);
            
    return 0;
}