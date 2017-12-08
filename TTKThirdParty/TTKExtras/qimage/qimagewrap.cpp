#include "qimagewrap.h"
#include "imagefilter.h"

#include <QImage>

QImage QImageWrap::GaussFilter(const QImage &image, int radius)
{
	QImage resultImage;
    if(image.isNull())
	{
		return resultImage;
	}

    resultImage = image.convertToFormat(QImage::Format_RGB888);

    static filter::pair_t pair[] =
    {
        { filter::Gauss, filter::Blur1D },
        { filter::Gauss2, filter::Blur2D },
        { filter::Linear, filter::Blur1D },
        { filter::Linear, filter::Blur2D },
        { filter::Average, filter::Blur1D },
        { filter::Average, filter::Blur2D }
    };

    filter::bitmap_t bmp;
    bmp.set((filter::bitmap_t::pixel_t*)resultImage.bits(),
                                        resultImage.width(),
                                        resultImage.height());
    filter::Filter(pair[4], bmp, radius);

    return resultImage;
}
