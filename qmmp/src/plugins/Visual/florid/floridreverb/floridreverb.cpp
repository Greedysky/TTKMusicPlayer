#include "floridreverb.h"
#include "inlines.h"

#include <QPainter>

FloridReverb::FloridReverb(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Reverb Widget"));
    m_gradientOn = true;
}

FloridReverb::~FloridReverb()
{
    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void FloridReverb::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);
    QPainter painter(this);
    draw(&painter);
}

void FloridReverb::process(float *left, float *)
{ 
    const int rows = (height() - 2) / m_cell_size.height();
    const int cols = (width() - 2) / m_cell_size.width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        if(m_x_scale)
        {
            delete[] m_x_scale;
        }

        m_intern_vis_data = new int[m_cols]{0};
        m_x_scale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    short y;
    int k, magnitude;

    calc_freq(dest, left);
    const double y_scale = (double) 1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        y = 0;
        magnitude = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            y = dest[i];
        }

        for(k = m_x_scale[i]; k < m_x_scale[i + 1]; ++k)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if(y)
        {
            magnitude = int(log(y) * y_scale);
            magnitude = qBound(0, magnitude, m_rows);
        }

        m_intern_vis_data[i] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}

void FloridReverb::draw(QPainter *p)
{
    if(m_rows == 0)
    {
        return;
    }

    p->setRenderHints(QPainter::Antialiasing);
    p->setPen(QPen(m_averageColor, 3));
    p->translate(rect().center());

    qreal startAngle = 0;
    for(int i = 0; i < DISTANCE; ++i)
    {
        p->save();
        p->rotate(startAngle);
        const double value = m_intern_vis_data[int(i * m_cols * 0.8 / DISTANCE)];
        p->drawLine(0, DISTANCE + 10, 0, DISTANCE + 10 + value * 0.3);

        p->restore();
        startAngle += 360.0 / DISTANCE;
    }
}
