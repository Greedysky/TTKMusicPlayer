#include "imagewrapper.h"
#include "random.h"

#include <qmath.h>
#include <QPainter>

namespace QAlgorithm
{
/*! @brief The class of the image render private.
 * @author Greedysky <greedysky@163.com>
 */
class ImageRenderPrivate : public TTKPrivate<ImageRender>
{
public:
    QRect m_rectangle;
};


ImageRender::ImageRender()
    : ImageRender(TTK_CREATE_PRIVATE(ImageRender))
{

}

ImageRender::ImageRender(ImageRenderPrivate &pvt)
{
    TTK_INIT_PRIVATE_D(pvt);
}

void ImageRender::input(const QRect &region)
{
    TTK_D(ImageRender);
    d->m_rectangle = region;
}

////////////////////////////////////////////////////////////////////////
GaussBlur::GaussBlur()
    : ImageRender()
{

}

QPixmap GaussBlur::render(const QPixmap &pixmap, int value)
{
    TTK_D(ImageRender);
    QImage image = pixmap.copy(d->m_rectangle).toImage();

    const float sigma = 1.0 * value / 2.57;
    const float deno = 1.0 / (sigma * sqrt(2.0 * M_PI));
    const float nume = -1.0 / (2.0 * sigma * sigma);

    float* gaussMatrix = (float*)malloc(sizeof(float)* (value + value + 1));
    float gaussSum = 0.0;
    for(int i = 0, x = -value; x <= value; ++x, ++i)
    {
        float g = deno * exp(1.0 * nume * x * x);

        gaussMatrix[i] = g;
        gaussSum += g;
    }

    const int len = value + value + 1;
    for(int i = 0; i < len; ++i)
    {
        gaussMatrix[i] /= gaussSum;
    }

    const int width = image.width();
    const int height = image.height();
    int *pix = (int*)image.bits();
    int *rowData = (int*)malloc(width * sizeof(int));
    int *listData = (int*)malloc(height * sizeof(int));

    for(int y = 0; y < height; ++y)
    {
        memcpy(rowData, pix + y * width, sizeof(int) *width);

        for(int x = 0; x < width; ++x)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for(int i = -value; i <= value; ++i)
            {
                int k = x + i;

                if(0 <= k && k <= width)
                {
                    int color = rowData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);

                    r += cr * gaussMatrix[i + value];
                    g += cg * gaussMatrix[i + value];
                    b += cb * gaussMatrix[i + value];

                    gaussSum += gaussMatrix[i + value];
                }
            }

            int cr = (int)(r / gaussSum);
            int cg = (int)(g / gaussSum);
            int cb = (int)(b / gaussSum);

            pix[y * width + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }

    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            listData[y] = pix[y * width + x];
        }

        for(int y = 0; y < height; ++y)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for(int j = -value; j <= value; ++j)
            {
                int k = y + j;

                if(0 <= k && k <= height)
                {
                    int color = listData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);

                    r += cr * gaussMatrix[j + value];
                    g += cg * gaussMatrix[j + value];
                    b += cb * gaussMatrix[j + value];

                    gaussSum += gaussMatrix[j + value];
                }
            }

            int cr = (int)(r / gaussSum);
            int cg = (int)(g / gaussSum);
            int cb = (int)(b / gaussSum);

            pix[y * width + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }

    free(gaussMatrix);
    free(rowData);
    free(listData);
    return QPixmap::fromImage(image);
}


////////////////////////////////////////////////////////////////////////
/*! @brief The class of the cube wave private.
 * @author Greedysky <greedysky@163.com>
 */
class CubeWavePrivate : public ImageRenderPrivate
{
public:
    CubeWavePrivate();

    void initialize(int width, int height);

    int count() const;
    bool isValid(int index, int value) const;

    int m_row;
    int m_column;
    TTKIntList m_data;
};

CubeWavePrivate::CubeWavePrivate()
    : ImageRenderPrivate(),
      m_row(0),
      m_column(0)
{
    QAlgorithm::initRandom();
}

void CubeWavePrivate::initialize(int width, int height)
{
    m_column = ceil(width * 1.0 / 8);
    m_row = ceil(height * 1.0  / 8);
}

bool CubeWavePrivate::isValid(int index, int value) const
{
    return (index < 0 || index > m_data.count()) ? false : (m_data[index] > value);
}

int CubeWavePrivate::count()  const
{
    return 8 * 8;
}


CubeWave::CubeWave()
    : ImageRender(TTK_CREATE_PRIVATE(CubeWave))
{

}

void CubeWave::input(const QRect &region)
{
    ImageRender::input(region);

    TTK_D(CubeWave);
    d->initialize(region.width(), region.height());
    for(int index = 0; index < d->count(); ++index)
    {
        d->m_data.push_back(QAlgorithm::random(100));
    }
}

QPixmap CubeWave::render(const QPixmap &pixmap, int value)
{
    TTK_D(CubeWave);
    QPixmap pix(d->m_rectangle.size());

    pix.fill(Qt::transparent);
    for(int index = 0; index < d->count(); ++index)
    {
        QPainter painter(&pix);

        const int row = index / 8;
        const int column = index % 8;

        QRect rect(QRect(row * d->m_column, column * d->m_row, d->m_column, d->m_row));
        if(rect.y() + rect.height() > pixmap.height())
        {
            rect.setHeight(pixmap.height() - rect.y());
        }

        if(d->isValid(index, value))
        {
            painter.fillRect(rect, QColor(0xFF, 0xFF, 0xFF, 255 - 2.55 * value));
        }

        painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
        painter.drawPixmap(rect, pixmap.copy(rect));
    }

    return pix;
}


////////////////////////////////////////////////////////////////////////
/*! @brief The class of the water wave private.
 * @author Greedysky <greedysky@163.com>
 */
class WaterWavePrivate : public ImageRenderPrivate
{
public:
    WaterWavePrivate();
    ~WaterWavePrivate();

    int* data();
    void render();
    void initialize(const QImage &image, int radius);

    void setWaveSourcePower(int depth);
    void setWaveSourcePosition(int x, int y);

private:
    void spreedRipple();
    void renderRipple();

private:
    int *m_orginPixels;
    int *m_newPixels;
    short *m_buffer1;
    short *m_buffer2;
    int *m_sourcePower;
    int *m_sourcePosition;

    int m_width;
    int m_height;

    int m_powerRate;

    int m_sourceRadius;
    int m_sourceDepth;
};

WaterWavePrivate::WaterWavePrivate()
    : ImageRenderPrivate()
{

}

WaterWavePrivate::~WaterWavePrivate()
{
    if(m_orginPixels)
    {
        delete[] m_orginPixels;
    }

    if(m_newPixels)
    {
        delete[] m_newPixels;
    }

    if(m_buffer1)
    {
        delete[] m_buffer1;
    }

    if(m_buffer2)
    {
        delete[] m_buffer2;
    }

    if(m_sourcePower)
    {
        delete[] m_sourcePower;
    }

    if(m_sourcePosition)
    {
        delete[] m_sourcePosition;
    }
}

int* WaterWavePrivate::data()
{
    return m_newPixels;
}

void WaterWavePrivate::render()
{
    spreedRipple();
    renderRipple();
}

void WaterWavePrivate::initialize(const QImage &image, int radius)
{
    m_orginPixels = nullptr;
    m_newPixels = nullptr;
    m_buffer1 = nullptr;
    m_buffer2 = nullptr;
    m_sourcePower = nullptr;
    m_sourcePosition = nullptr;

    m_width = image.width();
    m_height = image.height();

    m_orginPixels = new int[m_width * m_height]{};
    memcpy(m_orginPixels, image.bits(), m_width * m_height * 4);

    m_newPixels = new int[m_width * m_height]{};
    memcpy(m_newPixels, image.bits(), m_width * m_height * sizeof(int));

    m_buffer1 = new short[m_width * m_height]{};
    m_buffer2 = new short[m_width * m_height]{};

    m_powerRate = 3;
    m_sourceRadius = TTKStaticCast(int, radius);

    setWaveSourcePower(100);
}

void WaterWavePrivate::setWaveSourcePower(int depth)
{
    m_sourceDepth = depth;

    const int value = m_sourceRadius * m_sourceRadius;
    const int diameter = (m_sourceRadius << 1) + 1;
    const int rate = m_sourceRadius / value;
    const int size = diameter * diameter;

    m_sourcePower = new int[size]{};
    m_sourcePosition = new int[size]{};

    for(int x = 0; x <= diameter; ++x)
    {
        for(int y = 0; y <= diameter; ++y)
        {
            const int distanceSquare = (m_sourceRadius - x) * (m_sourceRadius - x) + (m_sourceRadius - y) * (m_sourceRadius - y);
            if(distanceSquare <= value)
            {
                const int depth = m_sourceDepth - distanceSquare * rate;
                m_sourcePosition[y * diameter + x] = y * m_width + x;
                m_sourcePower[y * diameter + x] = depth;
            }
        }
    }
}

void WaterWavePrivate::setWaveSourcePosition(int x, int y)
{
    const int sourceX = TTKStaticCast(int, x);
    const int sourceY = TTKStaticCast(int, y);
    if((sourceX + m_sourceRadius) >= m_width || (sourceY + m_sourceRadius) >= m_height || (sourceX - m_sourceRadius) <= 0 || (sourceY - m_sourceRadius) <= 0)
    {
        return;
    }

    const int distance = (sourceY - m_sourceRadius) * m_width + sourceX - m_sourceRadius;
    const int size = ((m_sourceRadius << 1) + 1) * ((m_sourceRadius << 1) + 1);
    for(int i = 0; i < size; ++i)
    {
        m_buffer1[distance + m_sourcePosition[i]] = TTKStaticCast(short, m_sourcePower[i]);
    }
}

void WaterWavePrivate::spreedRipple()
{
    const int length = m_width * (m_height - 1);
    for(int i = m_width; i < length; ++i)
    {
        m_buffer2[i] = ((m_buffer1[i - 1] + m_buffer1[i - m_width] + m_buffer1[i + 1] + m_buffer1[i + m_width]) >> 1) - m_buffer2[i];
        m_buffer2[i] -= m_buffer2[i] >> m_powerRate;
    }

    short* temp = m_buffer1;
    m_buffer1 = m_buffer2;
    m_buffer2 = temp;
}

void WaterWavePrivate::renderRipple()
{
    int offset;
    int w = m_width;
    for(int y = 1; y < m_height - 1; ++y)
    {
        for(int x = 0; x < m_width; ++x, ++w)
        {
            offset = (m_width * (m_buffer1[w - m_width] - m_buffer1[w + m_width])) + (m_buffer1[w - 1] - m_buffer1[w + 1]);
            if(w + offset > 0 && w + offset < m_width * m_height)
            {
                m_newPixels[w] = m_orginPixels[w + offset];
            }
            else
            {
                m_newPixels[w] = m_orginPixels[w];
            }
        }
    }
}


WaterWave::WaterWave(const QImage &image, int radius)
    : ImageRender(TTK_CREATE_PRIVATE(WaterWave))
{
    TTK_D(WaterWave);
    d->initialize(image, radius);
}

void WaterWave::input(const QRect &region)
{
    ImageRender::input(region);

    TTK_D(WaterWave);
    d->setWaveSourcePosition(region.width() / 2, region.height() / 2);
}

QPixmap WaterWave::render(const QPixmap &pixmap, int value)
{
    TTK_D(WaterWave);
    d->render();

    QImage image = pixmap.toImage();
    memcpy(image.bits(), (const uchar*)d->data(), QtImageBytes(image));

    QPixmap pix(d->m_rectangle.size());
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.fillRect(d->m_rectangle, QColor(0xFF, 0xFF, 0xFF, qMin(2.55 * 2 * value, 255.0)));
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(d->m_rectangle, QPixmap::fromImage(image));
    return pix;
}
}
