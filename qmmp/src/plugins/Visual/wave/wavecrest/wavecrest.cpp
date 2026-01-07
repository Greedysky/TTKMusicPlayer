#include "wavecrest.h"
#include "inlines.h"

#include <cmath>
#include <QPainter>

static constexpr int CHANNELS = 2;

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
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.fillRect(rect(), Qt::black);

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
        m_xscale = new int[CHANNELS]{0};

        m_backgroundImage = QImage(m_cols, m_rows, QImage::Format_RGB32);
        m_backgroundImage.fill(Qt::black);

        for(int i = 0; i < m_cols; ++i)
        {
            m_backgroundImage.setPixel(i, m_rows / 2, qRgb(0xff, 0xff, 0xff));
        }

        for(int i = 0; i < CHANNELS; ++i)
        {
            m_xscale[i] = std::pow(255.0, float(i) / m_cols);
        }
    }

    short destl[256];
    calc_freq(destl, left);

    short yl = 0;
    int i = 0, magnitudel = 0;
    const double yscale = (double)1.25 * m_rows / std::log(256);

    if(m_xscale[i] == m_xscale[i + 1])
    {
        yl = (i >= 256 ? 0 : (destl[i] >> 7)); //128
    }

    for(int k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
    {
        yl = (k >= 256 ? 0 : qMax(short(destl[k] >> 7), yl));
    }

    if(yl > 0)
    {
        magnitudel = qBound(0, int(std::log(yl) * yscale), m_rows);
    }

    m_visData -= m_analyzerSize * m_rows / 15;
    m_visData = magnitudel > m_visData ? magnitudel : m_visData;
}
