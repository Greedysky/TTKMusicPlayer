#include "floridbass.h"

#include <QPainter>

FloridBass::FloridBass(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Bass Widget"));

    m_timer->setInterval(QMMP_VISUAL_INTERVAL * 1.5);
}

void FloridBass::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);

    if(m_rows == 0)
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(QPen(m_averageColor, 2));
    painter.translate(rect().center());

    qreal startAngle = 0;
    for(int i = 0; i < m_rows * 2; ++i)
    {
        painter.save();
        painter.rotate(startAngle);

        int value1 = m_intern_vis_data[i];
        int value2 = m_intern_vis_data[i + 1];

        if(value1 > value2)
        {
            qSwap(value1, value2);
        }

        painter.drawLine(0, LABEL_RADIUS + 10 + value1 * 0.015, 0, LABEL_RADIUS + 10 + value2 * 0.015);

        if(value1 < 0)
        {
           value1 = 0;
        }
        painter.drawPoint(0, LABEL_RADIUS + 20 + value1 * 0.15);

        painter.restore();
        startAngle += 360.0 / (m_rows * 2);
    }
}

void FloridBass::processData(float *left, float *)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        m_intern_vis_data = new int[m_cols * 2]{0};
    }

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols * 2; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(left[pos >> 8] * m_rows / 2);
        m_intern_vis_data[i] = qBound(-m_rows / 2, m_intern_vis_data[i], m_rows / 2);
        m_intern_vis_data[m_cols * 2 - i - 1] = m_intern_vis_data[i];
    }
}
