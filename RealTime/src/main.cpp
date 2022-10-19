#include <stdio.h>
#include <stdlib.h>
#include "Movement.h"
#include "Tracking.h"
#include "Preprocessing.h"

#include "opencv2/opencv.hpp"

using namespace cv;

using namespace std;

int main(){
    Mat image;
    Mat image2;
    rgb8 ** rgbImage;
    long nrl,nrh,ncl,nch;
    char name[50];

    FILE* fichier = NULL;
 
    fichier = fopen("/dev/ttyACM0", "w");
    
    
    if (fichier != NULL)
    {   printf("fichier ouvert\n");
        fprintf(fichier,"%d",80);
        fclose(fichier);
    }

    namedWindow("Display window");
 
    VideoCapture cap(2);

    if (!cap.isOpened())
    {

        cout << "cannot open camera";

    }

    while (true)
    {
        // gettimeofday(&start,0);
        
        cap.read(image);
        rgbImage=convertMatToRGB8(image, &nrl, &nrh, &ncl, &nch);
        SavePPM_rgb8matrix(rgbImage,nrl,nrh,ncl,nch,"./saveRGB8.ppm");
        bool check = imwrite("./MyImage1.png", image);

        printf("ppm saved\n");
        free_rgb8matrix(rgbImage,nrl,nrh,ncl,nch);
        double fps =cap.get(CAP_PROP_FPS);
        // gettimeofday(&checkpoint,0);
        // int ms=diff_ms(checkpoint,start);
   
        // sprintf (name, "FPS : %f", 1/((double)ms/1000));
        sprintf (name, "FPS : %f", fps);
        putText(image, name, cv::Point(30,30), FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(20,20,25), 1, LINE_AA);
        imshow("Display window", image);

        break;
    }
    printf("FIN\n");
    


    return 0;
}