#ifndef __PREPROCESSING_H__
#define __PREPROCESSING_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv2/opencv.hpp>
using namespace cv;

#include "./NRC/def.h"
#include "./NRC/nrio.h"
#include "./NRC/nrarith.h"
#include "./NRC/nralloc.h"

void RGBtoHSV(byte _r, byte _g, byte _b, double *h, double *s, double *v);
byte** pickColor(rgb8 **I, byte r, byte g, byte b, long nrl, long nrh, long ncl, long nch);
rgb8** convertMatToRGB8(cv::Mat mat, long *nrl, long *nrh, long *ncl, long *nch);
byte** convertMatToByte(cv::Mat mat, long *nrl, long *nrh, long *ncl, long *nch);

#endif