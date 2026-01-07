#include "floridsurround.h"

#include <QPainter>

FloridSurround::FloridSurround(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Surround Widget"));
}

void FloridSurround::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);

    if(m_rows == 0)
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(rect().center());

    qreal startAngle = 0;
    for(int i = 0; i < m_rows * 2; ++i)
    {
        painter.save();
        painter.rotate(startAngle);

        int value1 = m_visualData[i];
        int value2 = m_visualData[i + 1];

        if(value1 > value2)
        {
            qSwap(value1, value2);
        }

        painter.setPen(QPen(m_averageColor, 2));
        painter.drawLine(0, LABEL_RADIUS + 10 + value1 * 0.03, 0, LABEL_RADIUS + 10 + value2 * 0.03);

        painter.setPen(QPen(QColor(m_averageColor.green(), m_averageColor.blue(), m_averageColor.red()), 2));
        painter.drawLine(0, LABEL_RADIUS + 10 + value1 * 0.06, 0, LABEL_RADIUS + 10 + value2 * 0.06);

        painter.setPen(QPen(QColor(m_averageColor.blue(), m_averageColor.red(), m_averageColor.green()), 2));
        painter.drawLine(0, LABEL_RADIUS + 10 + value1 * 0.09, 0, LABEL_RADIUS + 10 + value2 * 0.09);

        painter.restore();
        startAngle += 360.0 / (m_rows * 2);
    }
}

void FloridSurround::processData(float *left, float *)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_visualData;
        m_visualData = new int[m_cols * 2]{0};
    }

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols * 2; ++i)
    {
        pos += step;
        m_visualData[i] = int(left[pos >> 8] * m_rows / 2);
        m_visualData[i] = qBound(-m_rows / 2, m_visualData[i], m_rows / 2);
        m_visualData[m_cols * 2 - i - 1] = m_visualData[i];
    }
}
