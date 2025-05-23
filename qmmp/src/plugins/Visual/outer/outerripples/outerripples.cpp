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
    if(m_xscale)
    {
        delete[] m_xscale;
    }
}

void OuterRipples::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    QBrush brush(Qt::white, Qt::SolidPattern);
    for(int i = 0; i < m_cols; ++i)
    {
        const int x = i * m_cellSize.width() + 1;
        for(int j = 0; j <= m_visualData[i]; ++j)
        {
            painter.fillRect(x, height() - j * m_cellSize.height() + 1, m_cellSize.width() - 2, m_cellSize.height() - 2, brush);
        }
    }
}

void OuterRipples::processData(float *left, float *)
{
    const int rows = (height() - 2) / m_cellSize.height();
    const int cols = (width() - 2) / m_cellSize.width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_visualData)
        {
            delete[] m_visualData;
        }

        if(m_xscale)
        {
            delete[] m_xscale;
        }

        m_visualData = new int[m_cols]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    short y;
    int magnitude;

    calc_freq(dest, left);
    const double yscale = (double) 1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        y = 0;
        magnitude = 0;

        if(m_xscale[i] == m_xscale[i + 1])
        {
            y = dest[i];
        }

        for(int k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if(y)
        {
            magnitude = int(log(y) * yscale);
            magnitude = qBound(0, magnitude, m_rows);
        }

        m_visualData[i] -= m_analyzerSize * m_rows / 15;
        m_visualData[i] = magnitude > m_visualData[i] ? magnitude : m_visualData[i];
    }
}
