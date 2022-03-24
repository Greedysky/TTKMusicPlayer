#include "inlines.h"
#include "plusspacewave.h"

#include <QMenu>
#include <QPainter>
#include <math.h>

PlusSpaceWave::PlusSpaceWave(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Plus SpaceWave Widget"));
    setMinimumSize(2 * 300 - 30, 105);
}

PlusSpaceWave::~PlusSpaceWave()
{
    if(m_intern_ray_data)
    {
        delete[] m_intern_ray_data;
    }

    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void PlusSpaceWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    draw(&painter);
}

void PlusSpaceWave::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}

void PlusSpaceWave::process(float *left, float *)
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

        if(m_intern_ray_data)
        {
            delete[] m_intern_ray_data;
        }

        if(m_x_scale)
        {
            delete[] m_x_scale;
        }

        m_intern_vis_data = new int[m_cols]{0};
        m_intern_ray_data = new int[m_cols]{0};
        m_x_scale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    short y;
    int magnitude;

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

        for(int k = m_x_scale[i]; k < m_x_scale[i + 1]; ++k)
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

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols; ++i)
    {
        pos += step;
        m_intern_ray_data[i] = int(left[pos >> 8] * m_rows / 2);
        m_intern_ray_data[i] = qBound(-m_rows / 2, m_intern_ray_data[i], m_rows / 2);
    }
}

void PlusSpaceWave::draw(QPainter *p)
{
    QLinearGradient line(0, 0, width(), 0);
    line.setColorAt(1.0 * 1 / 7, QColor(242, 51, 255, 200));
    line.setColorAt(1.0 * 4 / 7, QColor(0, 186, 255, 200));
    line.setColorAt(1.0 * 6 / 7, QColor(242, 51, 255, 200));
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    int x = 0;
    for(int i = 0; i < m_cols; ++i)
    {
        x = i * (m_cell_size.width() + 7) + 1;
        const int offset = m_intern_vis_data[i] * m_cell_size.height() / 2;
        p->fillRect(x, height() / 2 - offset, (m_cell_size.width() + 7) - 1, offset, line);
    }

    p->setPen(QPen(QColor(0, 179, 255), 1));
    for(int i = 0; i < m_cols; ++i)
    {
        x = i * m_cell_size.width() * 2 + 1;
        const int offset = m_intern_vis_data[i] * m_cell_size.height() * 0.8;
        p->drawPoint(x, height() / 2 - offset);
        p->drawPoint(x, height() / 2 + offset);
    }
    //
    QLinearGradient ray_line(0, 0, width(), 0);
    ray_line.setColorAt(1.0 * 1 / 7, QColor(72, 176, 211));
    ray_line.setColorAt(1.0 * 2 / 7, QColor(57, 255, 57));
    ray_line.setColorAt(1.0 * 4 / 7, QColor(255, 247, 22));
    ray_line.setColorAt(1.0 * 5 / 7, QColor(255, 64, 59));
    ray_line.setColorAt(1.0 * 7 / 7, QColor(255, 64, 59));
    p->setPen(QPen(ray_line, 2));

    for(int i = 0; i < m_cols; ++i)
    {
        x = i * m_cell_size.width() + 1;
        if((i + 1) >= m_cols)
        {
            break;
        }

        int pFront = m_rows - m_intern_ray_data[i];
        int pEnd = m_rows - m_intern_ray_data[i + 1];

        if(pFront > pEnd)
        {
            qSwap(pFront, pEnd);
        }

        p->drawLine(x, pFront, x + 1, pEnd);
    }
}
