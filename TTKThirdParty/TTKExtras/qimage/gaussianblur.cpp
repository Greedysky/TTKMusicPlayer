#include "gaussianblur.h"

#include <cmath>
#include <iostream>

#define PI		3.14159265358979323846

class GaussianBlurPrivate : public TTKPrivate<GaussianBlur>
{
public:
    void boxBlurH(int* srcPix, int* destPix, int w, int h, int radius);
    void boxBlurV(int* srcPix, int* destPix, int w, int h, int radius);
    void boxBlur(int* srcPix, int* destPix, int w, int h, int r);
    void boxesForGauss(float sigma, int* size, int n);

};

void GaussianBlurPrivate::boxBlurH(int* srcPix, int* destPix, int w, int h, int radius)
{
    int index, r = 0, g = 0, b = 0;
    int tr, tg, tb;

    int color, preColor;

    int num;
    float iarr;

    for (int i = 0; i < h; ++i)
    {
        r = 0;
        g = 0;
        b = 0;

        index = i * w;
        num = radius;

        for (int j = 0; j < radius; j++)
        {
            color = srcPix[index + j];
            r += (color & 0x00ff0000) >> 16;
            g += (color & 0x0000ff00) >> 8;
            b += (color & 0x000000ff);
        }

        for (int j = 0; j <= radius; ++j)
        {
            num++;
            iarr = 1.0 / (1.0 * num);

            color = srcPix[index + j + radius];
            r += (color & 0x00ff0000) >> 16;
            g += (color & 0x0000ff00) >> 8;
            b += (color & 0x000000ff);

            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[index + j] = tr << 16 | tg << 8 | tb | 0xff000000;
        }

        iarr = 1.0 / (1.0 * num);
        for (int j = radius + 1; j < w - radius; ++j)
        {
            preColor = srcPix[index + j - 1 - radius];
            color = srcPix[index + j + radius];

            r = r + ((color & 0x00ff0000) >> 16) - ((preColor & 0x00ff0000) >> 16);
            g = g + ((color & 0x0000ff00) >> 8)  - ((preColor & 0x0000ff00) >> 8);
            b = b +  (color & 0x000000ff)        -  (preColor & 0x000000ff);

            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[index + j] = tr << 16 | tg << 8 | tb | 0xff000000;
        }

        for (int j = w - radius; j < w; ++j)
        {
            num--;
            iarr = 1.0 / (1.0 * num);

            preColor = srcPix[index + j - 1 - radius];

            r -= (preColor & 0x00ff0000) >> 16;
            g -= (preColor & 0x0000ff00) >> 8;
            b -= (preColor & 0x000000ff);

            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[index + j] = tr << 16 | tg << 8 | tb | 0xff000000;
        }
    }
}

void GaussianBlurPrivate::boxBlurV(int* srcPix, int* destPix, int w, int h, int radius)
{
    int r = 0, g = 0, b = 0;
    int tr, tg, tb;

    int color, preColor;

    int num;
    float iarr;

    for (int i = 0; i < w; ++i)
    {
        r = 0;
        g = 0;
        b = 0;

        num = radius;

        for (int j = 0; j < radius; ++j)
        {
            color = srcPix[j*w + i];
            r += (color & 0x00ff0000) >> 16;
            g += (color & 0x0000ff00) >> 8;
            b += (color & 0x000000ff);
        }

        for (int j = 0; j <= radius; ++j)
        {
            num++;
            iarr = 1.0 / (1.0 * num);

            color = srcPix[(j + radius) * w + i];
            r += (color & 0x00ff0000) >> 16;
            g += (color & 0x0000ff00) >> 8;
            b += (color & 0x000000ff);

            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[j*w + i] = tr << 16 | tg << 8 | tb | 0xff000000;
        }

        iarr = 1.0 / (1.0 * num);
        for (int j = radius + 1; j < h - radius; ++j)
        {
            preColor = srcPix[(j - radius - 1) * w + i];
            color = srcPix[(j + radius) * w + i];

            r = r + ((color & 0x00ff0000) >> 16) - ((preColor & 0x00ff0000) >> 16);
            g = g + ((color & 0x0000ff00) >> 8)  - ((preColor & 0x0000ff00) >> 8);
            b = b + (color & 0x000000ff)       - (preColor & 0x000000ff);

            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[j*w + i] = tr << 16 | tg << 8 | tb | 0xff000000;
        }

        for (int j = h - radius; j < h; ++j)
        {
            num--;
            iarr = 1.0 / (1.0 * num);
            preColor = srcPix[(j - radius - 1) * w + i];

            r -= (preColor & 0x00ff0000) >> 16;
            g -= (preColor & 0x0000ff00) >> 8;
            b -= (preColor & 0x000000ff);

            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[j*w + i] = tr << 16 | tg << 8 | tb | 0xff000000;
        }
    }
}

void GaussianBlurPrivate::boxBlur(int* srcPix, int* destPix, int w, int h, int r)
{
    if (r < 0)
    {
        return;
    }

    boxBlurH(srcPix, destPix, w, h, r);
    boxBlurV(destPix, srcPix, w, h, r);
}

void GaussianBlurPrivate::boxesForGauss(float sigma, int* size, int n)
{
    const float wIdeal = sqrt(12.0 * sigma * sigma / n + 1.0);
    int wl = floor(wIdeal);

    if (0 == wl % 2)
    {
        wl--;
    }

    const int wu = wl + 2;

    const float mIdeal = (12.0 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
    const int m = round(mIdeal);

    for (int i = 0; i < n; ++i)
    {
        size[i] = (i < m ? wl : wu);
    }
}



GaussianBlur::GaussianBlur()
{
    TTK_INIT_PRIVATE;
}

void GaussianBlur::gaussBlur(int* pix, int w, int h, int radius)
{
    const float sigma =  1.0 * radius / 2.57;
    const float deno  =  1.0 / (sigma * sqrt(2.0 * PI));
    const float nume  = -1.0 / (2.0 * sigma * sigma);

    float* gaussMatrix = (float*)malloc(sizeof(float)* (radius + radius + 1));
    float gaussSum = 0.0;
    for (int i = 0, x = -radius; x <= radius; ++x, ++i)
    {
        float g = deno * exp(1.0 * nume * x * x);

        gaussMatrix[i] = g;
        gaussSum += g;
    }

    const int len = radius + radius + 1;
    for (int i = 0; i < len; ++i)
    {
        gaussMatrix[i] /= gaussSum;
    }

    int* rowData  = (int*)malloc(w * sizeof(int));
    int* listData = (int*)malloc(h * sizeof(int));

    for (int y = 0; y < h; ++y)
    {
        memcpy(rowData, pix + y * w, sizeof(int) * w);

        for (int x = 0; x < w; ++x)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for (int i = -radius; i <= radius; ++i)
            {
                int k = x + i;

                if (0 <= k && k <= w)
                {
                    int color = rowData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);

                    r += cr * gaussMatrix[i + radius];
                    g += cg * gaussMatrix[i + radius];
                    b += cb * gaussMatrix[i + radius];

                    gaussSum += gaussMatrix[i + radius];
                }
            }

            int cr = (int)(r / gaussSum);
            int cg = (int)(g / gaussSum);
            int cb = (int)(b / gaussSum);

            pix[y * w + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }

    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            listData[y] = pix[y * w + x];
        }

        for (int y = 0; y < h; ++y)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for (int j = -radius; j <= radius; ++j)
            {
                int k = y + j;

                if (0 <= k && k <= h)
                {
                    int color = listData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);

                    r += cr * gaussMatrix[j + radius];
                    g += cg * gaussMatrix[j + radius];
                    b += cb * gaussMatrix[j + radius];

                    gaussSum += gaussMatrix[j + radius];
                }
            }

            int cr = (int)(r / gaussSum);
            int cg = (int)(g / gaussSum);
            int cb = (int)(b / gaussSum);

            pix[y * w + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }

    free(gaussMatrix);
    free(rowData);
    free(listData);
}


void GaussianBlur::gaussBlurPlus(int* pix, int w, int h, int radius)
{
    TTK_D(GaussianBlur);
    const float sigma = 1.0 * radius / 2.57;

    const int boxSize = 3;
    int* boxR = (int*)malloc(sizeof(int) * boxSize);

    d->boxesForGauss(sigma, boxR, boxSize);

    int* tempPix = (int*)malloc(sizeof(int) * w * h);

    d->boxBlur(pix, tempPix, w, h, (boxR[0] - 1) / 2);
    d->boxBlur(pix, tempPix, w, h, (boxR[1] - 1) / 2);
    d->boxBlur(pix, tempPix, w, h, (boxR[2] - 1) / 2);

    free(boxR);
    free(tempPix);
}
