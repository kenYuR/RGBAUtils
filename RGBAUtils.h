#ifndef __rgbautils_h
#define __rgbautils_h

#include <stdint.h>

namespace RGBAUtils
{

//填充
void Fill(uint16_t w, uint16_t h, uint8_t *data, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

//叠加
void Blend(uint16_t sw, uint16_t sh, uint8_t *sdata, uint16_t dw, uint16_t dh, uint8_t *ddata, uint16_t x, uint16_t y);

//裁剪
void Crop(uint16_t sw, uint16_t sh, uint8_t *sdata, uint16_t dw, uint16_t dh, uint8_t *ddata, uint16_t x, uint16_t y);

//缩放
void Scale(uint16_t sw, uint16_t sh, uint8_t *sdata, uint16_t dw, uint16_t dh, uint8_t *ddata, bool bilinear);

//旋转
void Rotate(uint16_t sw, uint16_t sh, uint8_t *sdata, uint8_t *ddata, double angle);

//镜像
void Flip(uint16_t sw, uint16_t sh, uint8_t *sdata, uint8_t *ddata, bool hflip, bool vflip);

}


#endif