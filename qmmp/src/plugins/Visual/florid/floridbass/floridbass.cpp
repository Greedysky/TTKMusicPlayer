#include "floridbass.h"

#include <QPainter>

FloridBass::FloridBass(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Bass Widget"));

    m_gradientOn = true;
    m_timer->setInterval(QMMP_VISUAL_INTERVAL * 1.5);
}

FloridBass::~FloridBass()
{

}

void FloridBass::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);
    QPainter painter(this);
    draw(&painter);
}

void FloridBass::process(float *left, float *)
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

void FloridBass::draw(QPainter *p)
{
    if(m_rows == 0)
    {
        return;
    }

    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p->setPen(QPen(m_averageColor, 2));
    p->translate(rect().center());

    qreal startAngle = 0;
    for(int i = 0; i < m_rows * 2; ++i)
    {
        p->save();
        p->rotate(startAngle);

        int value1 = m_intern_vis_data[i];
        int value2 = m_intern_vis_data[i + 1];
        if(value1 > value2)
        {
            qSwap(value1, value2);
        }

        p->drawLine(0, DISTANCE + 10 + value1 * 0.015, 0, DISTANCE + 10 + value2 * 0.015);

        if(value1 < 0)
        {
           value1 = 0;
        }
        p->drawPoint(0, DISTANCE + 20 + value1 * 0.15);

        p->restore();
        startAngle += 360.0 / (m_rows * 2);
    }
}
