#include "floridsurround.h"
#include "inlines.h"

#include <QPainter>

FloridSurround::FloridSurround(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Surround Widget"));
    m_gradientOn = true;
}

FloridSurround::~FloridSurround()
{

}

void FloridSurround::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);
    QPainter painter(this);
    draw(&painter);
}

void FloridSurround::process(float *left, float *)
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

void FloridSurround::draw(QPainter *p)
{
    if(m_rows == 0)
    {
        return;
    }

    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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

        p->setPen(QPen(m_averageColor, 2));
        p->drawLine(0, DISTANCE + 10 + value1 * 0.03, 0, DISTANCE + 10 + value2 * 0.03);

        p->setPen(QPen(QColor(m_averageColor.green(), m_averageColor.blue(), m_averageColor.red()), 2));
        p->drawLine(0, DISTANCE + 10 + value1 * 0.06, 0, DISTANCE + 10 + value2 * 0.06);

        p->setPen(QPen(QColor(m_averageColor.blue(), m_averageColor.red(), m_averageColor.green()), 2));
        p->drawLine(0, DISTANCE + 10 + value1 * 0.09, 0, DISTANCE + 10 + value2 * 0.09);

        p->restore();
        startAngle += 360.0 / (m_rows * 2);
    }
}
