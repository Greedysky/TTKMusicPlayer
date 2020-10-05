#include "musicimageutils.h"
#include "musicobject.h"
#include "qalg/qimagewrap.h"

#include <QBitmap>
#include <QBuffer>
#include <QPainter>

QPixmap MusicUtils::Image::pixmapToRound(const QPixmap &input, int ratioX, int ratioY)
{
    return pixmapToRound(input, QRect(QPoint(0, 0), input.size()), ratioX, ratioY);
}

QPixmap MusicUtils::Image::pixmapToRound(const QPixmap &input, const QSize &size, int ratioX, int ratioY)
{
    return pixmapToRound(input, QRect(QPoint(0, 0), size), ratioX, ratioY);
}

QPixmap MusicUtils::Image::pixmapToRound(const QPixmap &input, const QRect &rect, int ratioX, int ratioY)
{
    if(input.isNull())
    {
        return QPixmap();
    }

    QPixmap image = input.scaled(rect.size());
    image.setMask(getBitmapMask(rect, ratioX, ratioY));
    return image;
}

QPixmap MusicUtils::Image::pixmapToRound(const QPixmap &input, const QPixmap &mask, const QSize &size)
{
    if(input.isNull() || mask.isNull())
    {
        return QPixmap();
    }

    QPixmap image(mask);
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(0, 0, input.scaled(size));

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

QByteArray MusicUtils::Image::getPixmapData(const QPixmap &input)
{
    if(input.isNull())
    {
        return QByteArray();
    }

    QByteArray data;
    QBuffer buffer(&data);
    if(buffer.open(QIODevice::WriteOnly))
    {
        input.save(&buffer, JPG_FILE_PREFIX);
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

void MusicUtils::Image::fusionPixmap(QImage &back, const QImage &front, const QPoint &pt)
{
    if(front.isNull())
    {
        return;
    }

    QPainter painter(&back);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawImage(pt.x(), pt.y(), front);
}

QPixmap MusicUtils::Image::grayScalePixmap(const QPixmap &input, int radius)
{
    QImage pix = input.toImage();
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

int MusicUtils::Image::grayScaleAverage(const QImage &input, int width, int height)
{
    int average = 0;
    for(int w=0; w<width; w++)
    {
        for(int h=0; h<height; h++)
        {
            average += qBound(0, qGray(input.pixel(w, h)), 255);
        }
    }
    return average / (width * height);
}

QImage MusicUtils::Image::gaussPixmap(const QImage &input, int radius)
{
    QImage img = input.copy();
    MusicUtils::Image::gaussPixmap(img, radius);
    return img;
}

void MusicUtils::Image::gaussPixmap(QImage &input, int radius)
{
    QImageWrap::QGaussBlur wrap;
    wrap.render((int*)input.bits(), input.width(), input.height(), radius);
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
