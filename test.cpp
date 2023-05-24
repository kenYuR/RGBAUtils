#include "RGBAUtils.h"
#include <stdlib.h>
#include <stdio.h>

void TestFill()
{
    uint16_t w = 640;
    uint16_t h = 360;
    uint32_t size = w * h * 4;
    FILE *fp = NULL;
    uint8_t *data = (uint8_t*)malloc(size);
    //红
    RGBAUtils::Fill(w, h, data, 255, 0, 0, 255);
    fp = fopen("red360p.rgba", "wb");
    fwrite(data, 1, size, fp);
    fclose(fp);
    //绿
    RGBAUtils::Fill(w, h, data, 0, 255, 0, 255);
    fp = fopen("green360p.rgba", "wb");
    fwrite(data, 1, size, fp);
    fclose(fp);
    //蓝
    RGBAUtils::Fill(w, h, data, 0, 0, 255, 255);
    fp = fopen("blue360p.rgba", "wb");
    fwrite(data, 1, size, fp);
    fclose(fp);
    free(data);
}

void TestBlend()
{
    uint16_t sw = 640;
    uint16_t sh = 360;
    uint32_t ssize = sw * sh * 4;
    FILE *sfp = NULL;
    uint8_t *sdata = (uint8_t*)malloc(ssize);
    uint16_t dw = 1280;
    uint16_t dh = 720;
    uint32_t dsize = dw * dh * 4;
    FILE *dfp = NULL;
    uint8_t *ddata = (uint8_t*)malloc(dsize);
    dfp = fopen("blend720p.rgba", "wb");
    //合成红色
    sfp = fopen("red360p.rgba", "rb");
    fread(sdata, 1, ssize, sfp);
    fclose(sfp);
    RGBAUtils::Blend(sw, sh, sdata, dw, dh, ddata, 0, 0);
    //合成绿色
    sfp = fopen("green360p.rgba", "rb");
    fread(sdata, 1, ssize, sfp);
    fclose(sfp);
    RGBAUtils::Blend(sw, sh, sdata, dw, dh, ddata, 640, 0);
    //合成蓝色
    sfp = fopen("blue360p.rgba", "rb");
    fread(sdata, 1, ssize, sfp);
    fclose(sfp);
    RGBAUtils::Blend(sw, sh, sdata, dw, dh, ddata, 320, 360);
    fwrite(ddata, 1, dsize, dfp);
    free(sdata);
    free(ddata);
    fclose(dfp);
}

void TestCrop()
{
    uint16_t sw = 1280;
    uint16_t sh = 720;
    uint32_t ssize = sw * sh * 4;
    uint8_t *sdata = (uint8_t*)malloc(ssize);
    FILE *sfp = fopen("blend720p.rgba", "rb");
    uint16_t dw = 960;
    uint16_t dh = 540;
    uint32_t dsize = dw * dh * 4;
    uint8_t *ddata = (uint8_t*)malloc(dsize);
    FILE *dfp = fopen("crop540p.rgba", "wb");
    fread(sdata, 1, ssize, sfp);
    RGBAUtils::Crop(sw, sh, sdata, dw, dh, ddata, 160, 90);
    fwrite(ddata, 1, dsize, dfp);
    free(sdata);
    free(ddata);
    fclose(sfp);
    fclose(dfp);
}

void TestScaleDown()
{
    uint16_t sw = 1280;
    uint16_t sh = 720;
    uint32_t ssize = sw * sh * 4;
    uint8_t *sdata = (uint8_t*)malloc(ssize);
    FILE *sfp = fopen("blend720p.rgba", "rb");
    uint16_t dw = 640;
    uint16_t dh = 360;
    uint32_t dsize = dw * dh * 4;
    uint8_t *ddata = (uint8_t*)malloc(dsize);
    FILE *dfp = fopen("scaledown360p.rgba", "wb");
    fread(sdata, 1, ssize, sfp);
    RGBAUtils::Scale(sw, sh, sdata, dw, dh, ddata, true);
    fwrite(ddata, 1, dsize, dfp);
    free(sdata);
    free(ddata);
    fclose(sfp);
    fclose(dfp);
}

void TestScaleUp()
{
    uint16_t sw = 1280;
    uint16_t sh = 720;
    uint32_t ssize = sw * sh * 4;
    uint8_t *sdata = (uint8_t*)malloc(ssize);
    FILE *sfp = fopen("blend720p.rgba", "rb");
    uint16_t dw = 1920;
    uint16_t dh = 1080;
    uint32_t dsize = dw * dh * 4;
    uint8_t *ddata = (uint8_t*)malloc(dsize);
    FILE *dfp = fopen("scaleup1080p.rgba", "wb");
    fread(sdata, 1, ssize, sfp);
    RGBAUtils::Scale(sw, sh, sdata, dw, dh, ddata, true);
    fwrite(ddata, 1, dsize, dfp);
    free(sdata);
    free(ddata);
    fclose(sfp);
    fclose(dfp);
}

void TestRotate()
{
    uint16_t w = 1280;
    uint16_t h = 720;
    uint32_t size = w * h * 4;
    uint8_t *sdata = (uint8_t*)malloc(size);
    FILE *sfp = fopen("blend720p.rgba", "rb");
    uint8_t *ddata = (uint8_t*)malloc(size);
    FILE *dfp = fopen("rotate720p.rgba", "wb");
    fread(sdata, 1, size, sfp);
    RGBAUtils::Rotate(w, h, sdata, ddata, 45);
    fwrite(ddata, 1, size, dfp);
    free(sdata);
    free(ddata);
    fclose(sfp);
    fclose(dfp);
}

void TestHFlip()
{
    uint16_t w = 1280;
    uint16_t h = 720;
    uint32_t size = w * h * 4;
    uint8_t *sdata = (uint8_t*)malloc(size);
    FILE *sfp = fopen("blend720p.rgba", "rb");
    uint8_t *ddata = (uint8_t*)malloc(size);
    FILE *dfp = fopen("hflip720p.rgba", "wb");
    fread(sdata, 1, size, sfp);
    RGBAUtils::Flip(w, h, sdata, ddata, true, false);
    fwrite(ddata, 1, size, dfp);
    free(sdata);
    free(ddata);
    fclose(sfp);
    fclose(dfp); 
}

void TestVFlip()
{
    uint16_t w = 1280;
    uint16_t h = 720;
    uint32_t size = w * h * 4;
    uint8_t *sdata = (uint8_t*)malloc(size);
    FILE *sfp = fopen("blend720p.rgba", "rb");
    uint8_t *ddata = (uint8_t*)malloc(size);
    FILE *dfp = fopen("vflip720p.rgba", "wb");
    fread(sdata, 1, size, sfp);
    RGBAUtils::Flip(w, h, sdata, ddata, false, true);
    fwrite(ddata, 1, size, dfp);
    free(sdata);
    free(ddata);
    fclose(sfp);
    fclose(dfp); 
}

int main()
{
    TestFill();
    TestBlend();
    TestCrop();
    TestScaleDown();
    TestScaleUp();
    TestRotate();
    TestHFlip();
    TestVFlip();
    return 0;
}