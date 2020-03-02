#include "musicimageutils.h"
#include "musicobject.h"

#include <qmath.h>
#include <QBitmap>
#include <QBuffer>
#include <QPainter>

namespace GaussianBlur {
void gaussBlur(int* pix, int w, int h, int radius)
{
    const float sigma =  1.0 * radius / 2.57;
    const float deno  =  1.0 / (sigma * sqrt(2.0 * M_PI));
    const float nume  = -1.0 / (2.0 * sigma * sigma);

    float* gaussMatrix = (float*)malloc(sizeof(float)* (radius + radius + 1));
    float gaussSum = 0.0;
    for(int i = 0, x = -radius; x <= radius; ++x, ++i)
    {
        float g = deno * exp(1.0 * nume * x * x);

        gaussMatrix[i] = g;
        gaussSum += g;
    }

    const int len = radius + radius + 1;
    for(int i = 0; i < len; ++i)
    {
        gaussMatrix[i] /= gaussSum;
    }

    int* rowData  = (int*)malloc(w * sizeof(int));
    int* listData = (int*)malloc(h * sizeof(int));

    for(int y = 0; y < h; ++y)
    {
        memcpy(rowData, pix + y * w, sizeof(int) * w);

        for(int x = 0; x < w; ++x)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for(int i = -radius; i <= radius; ++i)
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

    for(int x = 0; x < w; ++x)
    {
        for(int y = 0; y < h; ++y)
        {
            listData[y] = pix[y * w + x];
        }

        for(int y = 0; y < h; ++y)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for(int j = -radius; j <= radius; ++j)
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
}


QPixmap MusicUtils::Image::pixmapToRound(const QPixmap &src, int ratioX, int ratioY)
{
    return pixmapToRound(src, QRect(QPoint(0, 0), src.size()), ratioX, ratioY);
}

QPixmap MusicUtils::Image::pixmapToRound(const QPixmap &src, const QSize &size, int ratioX, int ratioY)
{
    return pixmapToRound(src, QRect(QPoint(0, 0), size), ratioX, ratioY);
}

QPixmap MusicUtils::Image::pixmapToRound(const QPixmap &src, const QRect &rect, int ratioX, int ratioY)
{
    if(src.isNull())
    {
        return QPixmap();
    }

    QPixmap image = src.scaled(rect.size());
    image.setMask(getBitmapMask(rect, ratioX, ratioY));
    return image;
}

QPixmap MusicUtils::Image::pixmapToRound(const QPixmap &src, const QPixmap &mask, const QSize &size)
{
    if(src.isNull() || mask.isNull())
    {
        return QPixmap();
    }

    QPixmap image(mask);
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(0, 0, src.scaled(size));

    return image;
}

QBitmap MusicUtils::Image::getBitmapMask(const QRect &rect, int ratioX, int ratioY)
{
    QBitmap mask(rect.size());
    QPainter painter(&mask);
    painter.fillRect(rect, Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawRoundedRect(rect, ratioX, ratioY);
    return mask;
}

QByteArray MusicUtils::Image::getPixmapData(const QPixmap &pix)
{
    if(pix.isNull())
    {
        return QByteArray();
    }

    QByteArray data;
    QBuffer buffer(&data);
    if(buffer.open(QIODevice::WriteOnly))
    {
        pix.save(&buffer, JPG_FILE_PREFIX);
    }
    buffer.close();
    return data;
}

void MusicUtils::Image::fusionPixmap(QPixmap &back, const QPixmap &front, const QPoint &pt)
{
    if(front.isNull())
    {
        return;
    }

    QPainter painter(&back);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(pt.x(), pt.y(), front);
}

QPixmap MusicUtils::Image::grayScalePixmap(const QPixmap &src, int radius)
{
    QImage pix = src.toImage();
    for(int w=0; w<pix.width(); w++)
    {
        for(int h=0; h<pix.height(); h++)
        {
            int gray = qGray(pix.pixel(w, h)) + radius;
                gray = qBound(0, gray, 255);
            const QRgb rgb = qRgb(gray, gray, gray);
            pix.setPixel(w, h, rgb);
        }
    }
    return QPixmap::fromImage(pix);
}

QImage MusicUtils::Image::gaussPixmap(const QImage &image, int radius)
{
    QImage img = image.copy();
    MusicUtils::Image::gaussPixmap(img, radius);
    return img;
}

void MusicUtils::Image::gaussPixmap(QImage &image, int radius)
{
    GaussianBlur::gaussBlur((int*)image.bits(), image.width(), image.height(), radius);
}

int MusicUtils::Image::reRenderAlpha(int alpha, int value)
{
    return reRenderValue<int>(0xFF, alpha, value);
}

void MusicUtils::Image::reRenderImage(int delta, const QImage *input, QImage *output)
{
    for(int w=0; w<input->width(); w++)
    {
        for(int h=0; h<input->height(); h++)
        {
            const QRgb rgb = input->pixel(w, h);
            output->setPixel(w, h, qRgb(colorBurnTransform(qRed(rgb), delta),
                                        colorBurnTransform(qGreen(rgb), delta),
                                        colorBurnTransform(qBlue(rgb), delta)));
        }
    }
}

int MusicUtils::Image::colorBurnTransform(int c, int delta)
{
    if(0 > delta || delta > 0xFF)
    {
        return c;
    }

    const int result = (c - (c * delta)/(0xFF - delta));
    if(result > 0xFF)
    {
        return 0xFF;
    }
    else if(result < 0)
    {
        return 0;
    }
    return result;
}
