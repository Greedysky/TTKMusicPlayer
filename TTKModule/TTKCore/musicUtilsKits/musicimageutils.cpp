#include "musicimageutils.h"

#include <QBitmap>
#include <QBuffer>
#include <QPainter>

QPixmap TTK::Image::roundedPixmap(const QPixmap &input, int ratioX, int ratioY)
{
    return roundedPixmap(input, QRect(QPoint(0, 0), input.size()), ratioX, ratioY);
}

QPixmap TTK::Image::roundedPixmap(const QPixmap &input, const QSize &size, int ratioX, int ratioY)
{
    return roundedPixmap(input, QRect(QPoint(0, 0), size), ratioX, ratioY);
}

QPixmap TTK::Image::roundedPixmap(const QPixmap &input, const QRect &rect, int ratioX, int ratioY)
{
    if(input.isNull())
    {
        return QPixmap();
    }

    QPixmap image = input.scaled(rect.size());
    image.setMask(generateMask(rect, ratioX, ratioY));
    return image;
}

QPixmap TTK::Image::roundedPixmap(const QPixmap &input, const QPixmap &mask, const QSize &size)
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

QBitmap TTK::Image::generateMask(const QRect &rect, int ratioX, int ratioY)
{
    QBitmap mask(rect.size());
    QPainter painter(&mask);
    painter.fillRect(rect, Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawRoundedRect(rect, ratioX, ratioY);
    return mask;
}

QByteArray TTK::Image::generatePixmapData(const QPixmap &input)
{
    if(input.isNull())
    {
        return {};
    }

    QByteArray data;
    QBuffer buffer(&data);
    if(buffer.open(QIODevice::WriteOnly))
    {
        input.save(&buffer, JPG_FILE_SUFFIX);
    }
    buffer.close();
    return data;
}

void TTK::Image::fusionPixmap(QImage &back, const QImage &front, const QPoint &pt)
{
    if(front.isNull())
    {
        return;
    }

    QPainter painter(&back);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawImage(pt.x(), pt.y(), front);
}

void TTK::Image::fusionPixmap(QPixmap &back, const QPixmap &front, const QPoint &pt)
{
    if(front.isNull())
    {
        return;
    }

    QPainter painter(&back);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(pt.x(), pt.y(), front);
}

QRgb TTK::Image::colorContrast(const QRgb color)
{
    // Counting the perceptive luminance - human eye favors green color...
    int v = 255 - (2.0 * qRed(color) + 3.0 * qGreen(color) + qBlue(color)) / 6.0;
        v = v < 128 ? 0 : 255;
    // 0, bright colors; 255, dark colors
    return qRgb(v, v, v);
}

QPixmap TTK::Image::grayScalePixmap(const QPixmap &input, int radius)
{
    QImage pix = input.toImage();
    for(int w = 0; w < pix.width(); ++w)
    {
        for(int h = 0; h < pix.height(); ++h)
        {
            int gray = qGray(pix.pixel(w, h)) + radius;
                gray = qBound(0, gray, 255);
            const QRgb rgb = qRgb(gray, gray, gray);
            pix.setPixel(w, h, rgb);
        }
    }
    return QPixmap::fromImage(pix);
}

static int colorBurnTransform(int c, int delta)
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

void TTK::Image::reRenderImage(int delta, const QImage *input, QImage *output)
{
    for(int w = 0; w < input->width(); ++w)
    {
        for(int h = 0; h < input->height(); ++h)
        {
            const QRgb rgb = input->pixel(w, h);
            output->setPixel(w, h, qRgb(colorBurnTransform(qRed(rgb), delta),
                                        colorBurnTransform(qGreen(rgb), delta),
                                        colorBurnTransform(qBlue(rgb), delta)));
        }
    }
}
