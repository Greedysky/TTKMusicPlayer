#include "qimagewrap.h"
#include "gaussianblur.h"

#include <QImage>

QImage QImageWrap::GaussFilter(const QImage &image, int radius)
{
    QImage img = image.copy();
    GaussFilter(img, radius);
    return img;
}

void QImageWrap::GaussFilter(QImage &image, int radius)
{
    GaussianBlur().gaussBlur((int*)image.bits(), image.width(), image.height(), radius);
}
