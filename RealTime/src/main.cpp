#include <stdio.h>
#include <stdlib.h>
#include "Movement.h"
#include "Tracking.h"
#include "Preprocessing.h"
#include <sys/time.h>
#include "opencv2/opencv.hpp"
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
// #include <thread>

using namespace cv;

using namespace std;

// #define STABILISATION_MODE

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
    int h=90,v=90;
    FILE* fichier = NULL;
    timeval t1 ,t2;
    double ms;
    double fps;

    //gaussian filter parameters
    float sigma = 0.3;
    int size = 3;

    float lambda = sigma;


    double** filter = create_gaussian_filter(sigma, size);

    ofstream arduino;

	arduino.open( "/dev/ttyACM0");

    namedWindow("Camera");
 
    VideoCapture cap(0);

    if (!cap.isOpened())
    {
        perror("\t/!\\ CANNOT OPEN CAMERA\n");
    }
    cap.read(image);
    resize(image,imageResized,Size(896,504));

    rgbImage=convertMatToRGB8(image, &nrl, &nrh, &ncl, &nch);
    byteImage=convertMatToByte(imageResized, &nrl, &nrh, &ncl, &nch);
    harrisImage = dmatrix0(nrl, nrh, ncl, nch);

    // convert_rgb8_to_byte(rgbImage, byteImage, nrl, nrh, ncl, nch);
    // int quarter = (nrh+1) / 4;
    // std::thread th1(harris, byteImage, harrisImage, filter, size, lambda, nrl, nrh, ncl, nch, 0, quarter);
    // std::thread th2(harris, byteImage, harrisImage, filter, size, lambda, nrl, nrh, ncl, nch, quarter+1, 2*quarter);
    // std::thread th3(harris, byteImage, harrisImage, filter, size, lambda, nrl, nrh, ncl, nch, 2*quarter+1, 3*quarter);
    // std::thread th4(harris, byteImage, harrisImage, filter, size, lambda, nrl, nrh, ncl, nch, 3*quarter+1, nrh);
    // th1.join();
    // th2.join();
    // th3.join();
    // th4.join();
    convert_dmatrix_bmatrix(harrisImage, byteImage, nrl, nrh, ncl, nch);

    SavePGM_bmatrix(byteImage, nrl, nrh, ncl, nch, "harris_thread.pgm");

    while (true)
    {
        gettimeofday(&t1,0);

        cap.read(image);
        resize(image,imageResized,Size(896,504));
        
        // DEBUT TRAITEMENT
        #ifdef STABILISATION_MODE

        byteImage2=convertMatToByte(imageResized, &nrl, &nrh, &ncl, &nch);
        // convert_rgb8_to_byte(rgbImage2, byteImage2, nrl, nrh, ncl, nch);
      
        harrisImage2 = harris(byteImage2, filter, size, lambda, nrl, nrh, ncl, nch);
        convert_dmatrix_bmatrix(harrisImage2, byteImage2, nrl, nrh, ncl, nch);        
        Vecteur (byteImage, byteImage2, &x, &y, nrl, nrh, ncl, nch);
        // printf("x = %d, y = %d.\n",x,y);

        byteImage=byteImage2;
        #endif
        #ifndef STABILISATION_MODE
        rgbImage=convertMatToRGB8(imageResized, &nrl, &nrh, &ncl, &nch);
        getBarycenterColor(rgbImage,nrl,nrh,ncl,nch,&x,&y);

        #endif

       
        //FIN TRAITEMENT 

        gettimeofday(&t2,0);
        ms=diff_ms(t2,t1);
        fps=1.0/(ms/1000);

        
        // sprintf (name, "FPS : %f", 1/((double)ms/1000));
       
        circle(imageResized, Point(x,y), 3, Scalar(0, 255, 0));

        x=x-nrh/2;
        y=y-nch/2;
        
        sprintf (fpsDisplay, "FPS : %f     x: %d y:%d", fps,x,y);
        putText(imageResized, fpsDisplay, cv::Point(30,30), FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(20,20,25), 1, LINE_AA);
        imshow("Camera", imageResized);

        if(x>10)
        {
            h+=5;
        }else
        {
            if(x<-10)
            {
                h-=5;
            }

        }

        if(y>10)
        {
            v+=5;
        }else
        {
            if(y<-10)
            {
                v-=5;
            }

        }
        arduino<< setw(3)<<h<<setw(3)<<v<<endl;
        if(waitKey(33) == 27) break;

    }
    arduino.close();
        // free_rgb8matrix(rgbImage, nrl, nrh, ncl, nch);
        // free_rgb8matrix(rgbImage2, nrl, nrh, ncl, nch);

        // free_bmatrix  (byteImage, nrl, nrh, ncl, nch);
        // free_bmatrix  (byteImage2, nrl, nrh, ncl, nch);

        // free_dmatrix(harrisImage, nrl, nrh, ncl, nch);
        // free_dmatrix(harrisImage2, nrl, nrh, ncl, nch);
            
    return 0;
}