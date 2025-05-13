#include "floridreverb.h"
#include "inlines.h"

#include <QPainter>

FloridReverb::FloridReverb(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Reverb Widget"));
}

FloridReverb::~FloridReverb()
{
    if(m_xscale)
    {
        delete[] m_xscale;
    }
}

void FloridReverb::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);

    if(m_rows == 0)
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(QPen(m_averageColor, 3));
    painter.translate(rect().center());

    qreal startAngle = 0;
    for(int i = 0; i < LABEL_RADIUS; ++i)
    {
        painter.save();
        painter.rotate(startAngle);
        const double value = m_intern_vis_data[int(i * m_cols * 0.8 / LABEL_RADIUS)];
        painter.drawLine(0, LABEL_RADIUS + 10, 0, LABEL_RADIUS + 10 + value * 0.3);

        painter.restore();
        startAngle += 360.0 / LABEL_RADIUS;
    }
}

void FloridReverb::processData(float *left, float *)
{ 
    const int rows = (height() - 2) / m_cellSize.height();
    const int cols = (width() - 2) / m_cellSize.width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        if(m_xscale)
        {
            delete[] m_xscale;
        }

        m_intern_vis_data = new int[m_cols]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = pow(pow(255.0, 1.0 / m_cols), i);
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

        if(m_xscale[i] == m_xscale[i + 1])
        {
            y = dest[i];
        }

        for(k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if(y)
        {
            magnitude = int(log(y) * y_scale);
            magnitude = qBound(0, magnitude, m_rows);
        }

        m_intern_vis_data[i] -= m_analyzerSize * m_rows / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}
