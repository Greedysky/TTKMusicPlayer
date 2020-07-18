#include "qimagewrap.h"

#include <qmath.h>

namespace QImageWrap {
void QGaussBlur::render(int* pix, int width, int height, int radius)
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

    int* rowData  = (int*)malloc(width * sizeof(int));
    int* listData = (int*)malloc(height * sizeof(int));

    for(int y = 0; y < height; ++y)
    {
        memcpy(rowData, pix + y * width, sizeof(int) *width);

        for(int x = 0; x < width; ++x)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for(int i = -radius; i <= radius; ++i)
            {
                int k = x + i;

                if(0 <= k && k <= width)
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

            for(int j = -radius; j <= radius; ++j)
            {
                int k = y + j;

                if(0 <= k && k <= height)
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

            pix[y * width + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }

    free(gaussMatrix);
    free(rowData);
    free(listData);
}


////////////////////////////////////////////////////////////////////////
class QWaterWavePrivate : public TTKPrivate<QWaterWave>
{
public:
    QWaterWavePrivate();
    ~QWaterWavePrivate();

    int* data();
    void render();
    void init(const QImage &image, int radius);

    void setWaveSourcePower(int radius, int depth);
    void setWaveSourcePosition(int x, int y);

private:
    void spreedRipple();
    void renderRipple();

private:
    int* m_orginPixels;
    int* m_newPixels;
    short* m_buffer1;
    short* m_buffer2;
    int* m_sourcePower;
    int* m_sourcePosition;

    int m_width;
    int m_height;

    int m_powerRate;
    float m_scale;

    int m_sourceRadius;
    int m_sourceDepth;
};


QWaterWavePrivate::QWaterWavePrivate()
{

}

QWaterWavePrivate::~QWaterWavePrivate()
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

int* QWaterWavePrivate::data()
{
    return m_newPixels;
}

void QWaterWavePrivate::render()
{
    spreedRipple();
    renderRipple();
}

void QWaterWavePrivate::init(const QImage &image, int radius)
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

    m_powerRate = 2;
    m_scale = 1;

    setWaveSourcePower(radius, 100);
}

void QWaterWavePrivate::setWaveSourcePower(int radius, int depth)
{
    m_sourceRadius = static_cast<int>(radius / m_scale);
    m_sourceDepth = depth;

    const int value = m_sourceRadius * m_sourceRadius;
    const int diameter = (m_sourceRadius << 1) + 1;
    const int rate = m_sourceRadius / value;

    const int size = diameter*diameter;
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

void QWaterWavePrivate::setWaveSourcePosition(int x, int y)
{
    const int sourceX = static_cast<int>(x / m_scale);
    const int sourceY = static_cast<int>(y / m_scale);
    if((sourceX + m_sourceRadius) >= m_width || (sourceY + m_sourceRadius) >= m_height || (sourceX - m_sourceRadius) <= 0 || (sourceY - m_sourceRadius) <= 0)
    {
        return;
    }

    const int distance = (sourceY - m_sourceRadius) * m_width + sourceX - m_sourceRadius;
    const int size = ((m_sourceRadius << 1) + 1) * ((m_sourceRadius << 1) + 1);
    for(int i = 0; i < size; ++i)
    {
        m_buffer1[distance + m_sourcePosition[i]] = static_cast<short>(m_sourcePower[i]);
    }
}

void QWaterWavePrivate::spreedRipple()
{
    const int length = m_width*(m_height -1);
    for(int i = m_width; i < length; ++i)
    {
        m_buffer2[i] = ((m_buffer1[i - 1] + m_buffer1[i - m_width] + m_buffer1[i + 1] + m_buffer1[i + m_width]) >> 1) - m_buffer2[i];
        m_buffer2[i] -= m_buffer2[i] >> m_powerRate;
    }

    short* temp = m_buffer1;
    m_buffer1 = m_buffer2;
    m_buffer2 = temp;
}

void QWaterWavePrivate::renderRipple()
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


QWaterWave::QWaterWave(const QImage &image, int radius)
{
    TTK_INIT_PRIVATE;
    TTK_D(QWaterWave);
    d->init(image, radius);
}

int* QWaterWave::data()
{
    TTK_D(QWaterWave);
    return d->data();
}

void QWaterWave::render()
{
    TTK_D(QWaterWave);
    d->render();
}

void QWaterWave::input(int x, int y)
{
    TTK_D(QWaterWave);
    d->setWaveSourcePosition(x, y);
}


////////////////////////////////////////////////////////////////////////
}
