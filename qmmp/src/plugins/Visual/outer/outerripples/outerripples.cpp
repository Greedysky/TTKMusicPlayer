#include "outerripples.h"
#include "inlines.h"

#include <QPainter>
#include <math.h>

OuterRipples::OuterRipples(QWidget *parent)
    : Visual(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setWindowTitle(tr("Outer Ripples Widget"));
}

OuterRipples::~OuterRipples()
{
    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void OuterRipples::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    draw(&painter);
}

void OuterRipples::process(float *left, float *)
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
}

void OuterRipples::draw(QPainter *p)
{
    QBrush brush(Qt::white, Qt::SolidPattern);

    int x = 0;
    for(int i = 0; i < m_cols; ++i)
    {
        x = i * m_cell_size.width() + 1;
        for(int j = 0; j <= m_intern_vis_data[i]; ++j)
        {
            p->fillRect(x, height() - j * m_cell_size.height() + 1, m_cell_size.width() - 2, m_cell_size.height() - 2, brush);
        }
    }
}
