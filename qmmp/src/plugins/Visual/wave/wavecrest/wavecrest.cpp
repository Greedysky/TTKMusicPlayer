#include "wavecrest.h"
#include "inlines.h"

#include <QPainter>
#include <math.h>

WaveCrest::WaveCrest(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Wave Crest Widget"));
    setMinimumSize(2 * 300 - 30, 105);
}

WaveCrest::~WaveCrest()
{
    delete[] m_xscale;
}

void WaveCrest::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if(m_backgroundImage.isNull())
    {
        return;
    }

    if(m_cols != 0)
    {
        if(m_offset >= m_cols)
        {
            m_offset = m_cols - 1;
            m_backgroundImage = m_backgroundImage.copy(1, 0, m_cols, m_rows);
        }

        for(int i = 0; i < m_visData / 2; ++i)
        {
            const int r = qMin(0x5f + i * 3, 0xff);
            m_backgroundImage.setPixel(m_offset, qMax(m_rows / 2 - i, 0), qRgb(r, r, r));
            m_backgroundImage.setPixel(m_offset, qMin(m_rows / 2 + i, m_rows), qRgb(r, r, r));
        }
        m_backgroundImage.setPixel(m_offset, m_rows / 2, qRgb(0xff, 0xff, 0xff));
    }

    ++m_offset;
    painter.drawImage(0, 0, m_backgroundImage);
}

void WaveCrest::processData(float *left, float *)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        m_offset = 0;

        delete[] m_xscale;

        m_visData = 0;
        m_xscale = new int[2];

        m_backgroundImage = QImage(m_cols, m_rows, QImage::Format_RGB32);
        m_backgroundImage.fill(Qt::black);

        for(int i = 0; i < m_cols; ++i)
        {
            m_backgroundImage.setPixel(i, m_rows / 2, qRgb(0xff, 0xff, 0xff));
        }

        for(int i = 0; i < 2; ++i)
        {
            m_xscale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    calc_freq(dest, left);

    short y = 0;
    int magnitude = 0;
    const double yscale = (double) 1.25 * m_rows / log(256);

    if(m_xscale[0] == m_xscale[1])
    {
        y = dest[0];
    }

    for(int k = m_xscale[0]; k < m_xscale[1]; ++k)
    {
        y = qMax(dest[k], y);
    }

    y >>= 7; //256

    if(y)
    {
        magnitude = int(log(y) * yscale);
        magnitude = qBound(0, magnitude, m_rows);
    }

    m_visData -= m_analyzerSize * m_rows / 15;
    m_visData = magnitude > m_visData ? magnitude : m_visData;
}
