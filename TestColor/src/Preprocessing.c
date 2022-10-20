#include "Preprocessing.h"

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


int isRed(rgb8 pxl){
    uint8 red = pxl.r;
    uint8 green = pxl.g;
    uint8 blue = pxl.b;
    float alpha = 0.1; //parametre de seuil
    int sum = red + green +blue;

    float redRatio = red/sum;
    float greenRatio = green/sum;
    float blueRatio = blue/sum;

    (redRatio > blueRatio + alpha) && (redRatio > greenRatio + alpha) ? 1 : 0;
}