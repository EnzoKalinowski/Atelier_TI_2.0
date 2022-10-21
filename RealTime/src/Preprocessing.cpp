#include "Preprocessing.h"

/**
 * @brief function to convert rgb representation to hue saturation value (hsv) representation
 *
 * @param _r red part of the pxl
 * @param _g green part of the pxl
 * @param _b blue part of the pxl
 * @param h hue param
 * @param s saturation
 * @param v value
 */
void RGBtoHSV(byte _r, byte _g, byte _b, double *h, double *s, double *v){
    double r = (double) _r / 255;
    double g = (double) _g / 255;
    double b = (double) _b / 255;
    
    double cmax = r;
    if(g > cmax)
        cmax = g;
    if(b > cmax)
        cmax = b;

    double cmin = r;
    if(g < cmin)
        cmin = g;
    if(b < cmin)
        cmin = b;

    double delta = cmax - cmin;

    if(delta == 0){
        *h = 0;
    }else if (cmax == r){
        *h = 60 * ((g - b) / delta);
    }else if (cmax == g){
        *h = 60 * ((b - r) / delta + 2);
    }else if (cmax == b){
        *h = 60 * ((r - g) / delta + 4);
    }
    
    *s = (cmax == 0) ? 0 : delta/cmax;
    *v = cmax;
}

/**
 * @brief
 *
 * @param I
 * @param r
 * @param g
 * @param b
 * @param nrl
 * @param nrh
 * @param ncl
 * @param nch
 * @return
 */
byte** pickColorHSV(rgb8 **I, byte r, byte g, byte b, long nrl, long nrh, long ncl, long nch){
    double _h, _s, _v, h, s, v;
    RGBtoHSV(r, g, b, &_h, &_s, &_v);

    byte **res = bmatrix(nrl, nrh, ncl, nch);

    rgb8 col;
    for(int i = nrl; i < nrh; i++){
        for(int j = ncl; j < nch; j++){
            col = I[i][j];
            RGBtoHSV(col.r, col.g, col.b, &h, &s, &v);
            if((_h-10 < h && h < _h+10) && (_s-0.1 < s && s < _s+0.1)){
                res[i][j] = 255;
            }else{
                res[i][j] = 0;
            }
        }
    }

    return res;
}

/**
 * @brief
 *
 * @param I
 * @param r
 * @param g
 * @param b
 * @param nrl
 * @param nrh
 * @param ncl
 * @param nch
 * @return
 */
byte** pickColorRGB(rgb8 **I, byte r, byte g, byte b, long nrl, long nrh, long ncl, long nch){
    byte **res = bmatrix(nrl, nrh, ncl, nch);
    rgb8 col;
    double threshold = 0.2;
    double _total, total;
    double rr, rg, rb, _rr, _rg, _rb;

    _total = r + g + b;
    _rr = r/_total;
    _rg = g/_total;
    _rb = b/_total;

    for(int i = nrl; i < nrh; i++){
        for(int j = ncl; j < nch; j++){
            col = I[i][j];
            total = col.r + col.g + col.b;
            rr = col.r/total;
            rg = col.g/total;
            rb = col.b/total;

            if((_rr-threshold < rr && rr < _rr+threshold) && (_rg-threshold < rg && rg < _rg+threshold) && (_rb-threshold < rb && rb < _rb+threshold)){
                res[i][j] = 255;
            }else{
                res[i][j] = 0;
            }
        }
    }

    return res;
}

/**
 * @brief converts a cv matrix to an rgb8_matrix from nrc lib
 *
 * @param mat cv mat
 * @param nrl number row low (lowest index of rows)
 * @param nrh number row high (highest index of rows)
 * @param ncl number column low (lowest index of col)
 * @param nch number column high (highest index of col)
 * @return an rgb8matrix
 */
rgb8** convertMatToRGB8(cv::Mat mat, long *nrl, long *nrh, long *ncl, long *nch){
    int width = mat.cols;
    int height = mat.rows;
    Vec3b color;

    *nrl = 0;
    *nrh = height-1;
    *ncl = 0;
    *nch = width-1;
    rgb8 **out = rgb8matrix(*nrl, *nrh, *ncl, *nch);

    for(int i = *nrl; i < *nrh; i++){
        for(int j = *ncl; j < *nch; j++){
            color = mat.at<Vec3b>(Point(j, i));
            out[i][j].r = color[2];
            out[i][j].g = color[1];
            out[i][j].b = color[0];
        }
    }

    return out;
}

/**
 * @brief converts a cv matrix to byte matrix from nrc lib
 *
 * @param mat cv mat
 * @param nrl number row low (lowest index of rows)
 * @param nrh number row high (highest index of rows)
 * @param ncl number column low (lowest index of col)
 * @param nch number column high (highest index of col)
 *
 * @return a bmatrix
 */
byte** convertMatToByte(cv::Mat mat, long *nrl, long *nrh, long *ncl, long *nch){
    int width = mat.cols;
    int height = mat.rows;
    int grey;
    Vec3b color;

    *nrl = 0;
    *nrh = height-1;
    *ncl = 0;
    *nch = width-1;
    byte **out = bmatrix(*nrl, *nrh, *ncl, *nch);

    for(int i = *nrl; i < *nrh; i++){
        for(int j = *ncl; j < *nch; j++){
            color = mat.at<Vec3b>(Point(j, i));
            grey = (color[0] + color[1] + color[2]) / 3;
            out[i][j] = grey;
        }
    }

    return out;
}

/**
 * @brief return 1 if the pixel is of the good color
 *
 * @param pxl rgb pixel
 * @param r red component
 * @param g green component
 * @param b blue component
 * @return 1 if the pixel is more of the color chosen, 0 if not
 */
int isOfColor(rgb8 pxl, byte r, byte g, byte b){
    double threshold = 0.1;
    double targetTotal = r + g + b;
    double targetR = r / targetTotal;
    double targetG = g / targetTotal;
    double targetB = b / targetTotal;

    double total = pxl.r + pxl.g + pxl.b + 10e-9;
    double pxlR = pxl.r / total;
    double pxlG = pxl.g / total;
    double pxlB = pxl.b / total;

    if((targetR-threshold < pxlR && pxlR < targetR+threshold) && (targetG-threshold < pxlG && pxlG < targetG+threshold) && (targetB-threshold < pxlB && pxlB < targetB+threshold)){
        return 1;
    }else{
        return 0;
    }
}