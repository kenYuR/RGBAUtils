#include "RGBAUtils.h"
#include <string.h>
#include <cmath>
#include <stdio.h>

namespace RGBAUtils
{

typedef struct 
{
    float m[2];
}Mat1x2;

typedef struct 
{
    float m[2][2];
}Mat2x2;

void MatTrans(Mat1x2 srcMat, Mat2x2 transMat, Mat1x2 &dstMat)
{
    dstMat.m[0] = srcMat.m[0] * transMat.m[0][0] + srcMat.m[1] * transMat.m[1][0];
    dstMat.m[1] = srcMat.m[0] * transMat.m[0][1] + srcMat.m[1] * transMat.m[1][1];
}

void Fill(uint16_t w, uint16_t h, uint8_t *data, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    for(uint16_t y=0; y<h; y++){
        for(uint16_t x=0; x<w; x++){
            uint32_t idx = (y * w + x) * 4;
            data[idx + 0] = r;
            data[idx + 1] = g;
            data[idx + 2] = b;
            data[idx + 3] = a;
        }
    }
}

void Blend(uint16_t sw, uint16_t sh, uint8_t *sdata, uint16_t dw, uint16_t dh, uint8_t *ddata, uint16_t x, uint16_t y)
{
    uint16_t dx = x;
    uint16_t dy = y;
    for(uint16_t sy=0; sy<sh; sy++){
        memcpy(ddata + (dy*dw+dx)*4, sdata + sy*sw*4, sw*4);
        dy++;
    }
}

void Crop(uint16_t sw, uint16_t sh, uint8_t *sdata, uint16_t dw, uint16_t dh, uint8_t *ddata, uint16_t x, uint16_t y)
{
    uint16_t sx = x;
    uint16_t sy = y;
    for(uint16_t dy=0; dy<dh; dy++){
        memcpy(ddata + dy*dw*4, sdata + (sy*sw+sx)*4, dw*4);
        sy++;
    }
}

void Scale(uint16_t sw, uint16_t sh, uint8_t *sdata, uint16_t dw, uint16_t dh, uint8_t *ddata, bool bilinear)
{
    Mat2x2 trans;
    trans.m[0][0] = (float)sw / (float)dw; trans.m[0][1] = 0;
    trans.m[1][0] = 0; trans.m[1][1] = (float)sh / (float)dh;
    for(uint16_t dy=0; dy<dh; dy++){
        for(uint16_t dx=0; dx<dw; dx++){
            uint32_t didx = (dy*dw+dx)*4;
            uint8_t rgba[4] = { 0 };
            Mat1x2 dCurrent;
            dCurrent.m[0] = dx; dCurrent.m[1] = dy;
            Mat1x2 dNext;
            dNext.m[0] = dx + 1; dNext.m[1] = dy + 1;
            Mat1x2 sCurrent;
            Mat1x2 sNext;
            MatTrans(dCurrent, trans, sCurrent);
            MatTrans(dNext, trans, sNext);
            if(bilinear){
                for(uint16_t sy=sCurrent.m[1]; sy<sNext.m[1]; sy++){
                    for(uint16_t sx=sCurrent.m[0]; sx<sNext.m[0]; sx++){
                        uint32_t sidx = (sy*sw+sx)*4;
                        rgba[0] = (rgba[0] + sdata[sidx+0]) / 2;
                        rgba[1] = (rgba[1] + sdata[sidx+1]) / 2;
                        rgba[2] = (rgba[2] + sdata[sidx+2]) / 2;
                        rgba[3] = (rgba[3] + sdata[sidx+3]) / 2;
                    }
                }
            }else{
                uint16_t sx = sCurrent.m[0];
                uint16_t sy = sCurrent.m[1];
                uint32_t sidx = (sy*sw+sx)*4;
                rgba[0] = sdata[sidx+0];
                rgba[1] = sdata[sidx+1];
                rgba[2] = sdata[sidx+2];
                rgba[3] = sdata[sidx+3];
            }
            ddata[didx+0] = rgba[0];
            ddata[didx+1] = rgba[1];
            ddata[didx+2] = rgba[2];
            ddata[didx+3] = rgba[3];
        }
    }
}

void Rotate(uint16_t sw, uint16_t sh, uint8_t *sdata, uint8_t *ddata, double angle)
{
    memset(ddata, 0, sw * sh * 4);
    double pi = 3.1415926;
    double radian = (0 - angle) / 180.0 * pi;
    Mat2x2 trans;
    trans.m[0][0] = cos(radian); trans.m[0][1] = sin(radian);
    trans.m[1][0] = -sin(radian); trans.m[1][1] = cos(radian);
    Mat1x2 center;
    center.m[0] = sw / 2;
    center.m[1] = sh / 2;
    for(uint16_t dy=0; dy<sh; dy++){
        for(uint16_t dx=0; dx<sw; dx++){
            uint32_t didx = (dy*sw+dx)*4;
            Mat1x2 dCurrent;
            dCurrent.m[0] = dx - center.m[0];
            dCurrent.m[1] = dy - center.m[1];
            Mat1x2 sCurrent;
            MatTrans(dCurrent, trans, sCurrent);
            float fsx = sCurrent.m[0] + center.m[0];
            float fsy = sCurrent.m[1] + center.m[1];
            if((fsx >=0 && fsx < sw) && (fsy >= 0 && fsy < sh)){
                uint16_t sx = fsx;
                uint16_t sy = fsy;
                uint32_t sidx = (sy*sw+sx)*4;
                ddata[didx+0] = sdata[sidx+0];
                ddata[didx+1] = sdata[sidx+1];
                ddata[didx+2] = sdata[sidx+2];
                ddata[didx+3] = sdata[sidx+3];
            }
        }
    }
}

void Flip(uint16_t sw, uint16_t sh, uint8_t *sdata, uint8_t *ddata, bool hflip, bool vflip)
{
    for(uint16_t dy=0; dy<sh; dy++){
        for(uint16_t dx=0; dx<sw; dx++){
            uint16_t sx = hflip ? sw - 1 - dx : dx;
            uint16_t sy = vflip ? sh - 1 - dy : dy;
            uint32_t sidx = (sy*sw+sx)*4;
            uint32_t didx = (dy*sw+dx)*4;
            ddata[didx+0] = sdata[sidx+0];
            ddata[didx+1] = sdata[sidx+1];
            ddata[didx+2] = sdata[sidx+2];
            ddata[didx+3] = sdata[sidx+3];
        }
    }
}

}