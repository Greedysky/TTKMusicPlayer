#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>

#include "fft.h"
#include "inlines.h"
#include "wavemulti.h"

WaveMulti::WaveMulti(QWidget *parent)
    : Visual(parent)
{
    m_vis_data = 0;
    m_x_scale = nullptr;
    m_analyzer_falloff = 2.2;
    m_pixPos = 0;

    setWindowTitle(tr("Wave Multi Widget"));
    setMinimumSize(2*300-30, 105);
}

WaveMulti::~WaveMulti()
{
    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void WaveMulti::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void WaveMulti::process(float *left, float *)
{
    static fft_state *state = nullptr;
    if(!state)
    {
        state = fft_init();
    }

    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        m_pixPos = 0;

        if(m_x_scale)
        {
            delete[] m_x_scale;
        }

        m_vis_data = 0;
        m_x_scale = new int[2];

        m_backgroundImage = QImage(m_cols, m_rows, QImage::Format_RGB32);
        m_backgroundImage.fill(Qt::black);

        for(int i = 0; i < m_cols; ++i)
        {
            m_backgroundImage.setPixel(i, m_rows / 2, qRgb(0xff, 0xff, 0xff));
        }

        for(int i = 0; i < 2; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    short y = 0;
    int k, magnitude = 0;

    calc_freq(dest, left);
    const double y_scale = (double) 1.25 * m_rows / log(256);

    if(m_x_scale[0] == m_x_scale[1])
    {
        y = dest[0];
    }

    for(k = m_x_scale[0]; k < m_x_scale[1]; k++)
    {
        y = qMax(dest[k], y);
    }

    y >>= 7; //256

    if(y)
    {
        magnitude = int(log(y) * y_scale);
        magnitude = qBound(0, magnitude, m_rows);
    }

    m_vis_data -= m_analyzer_falloff * m_rows / 15;
    m_vis_data = magnitude > m_vis_data ? magnitude : m_vis_data;
}

void WaveMulti::draw(QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    const float maxed = takeMaxRange();

    if(m_cols != 0)
    {
        if(m_pixPos >= m_cols)
        {
            m_pixPos = m_cols - 1;
            m_backgroundImage = m_backgroundImage.copy(1, 0, m_cols, m_rows);
        }

        for(int i=0; i<m_vis_data * maxed / 2; ++i)
        {
            int r = qMin(0x5f + i*3, 0xff);
            m_backgroundImage.setPixel(m_pixPos, qMax(m_rows / 2 - i, 0), qRgb(r, 0, 0));
            m_backgroundImage.setPixel(m_pixPos, qMin(m_rows / 2 + i, m_rows), qRgb(r, 0, 0));
        }
        m_backgroundImage.setPixel(m_pixPos, m_rows / 2, qRgb(0xff, 0xff, 0xff));
    }
    ++m_pixPos;

    if(!m_backgroundImage.isNull())
    {
        p->drawImage(0, 0, m_backgroundImage);
    }
}
