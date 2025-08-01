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
    delete[] m_xscale;
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

        delete[] m_visualData;
        delete[] m_xscale;

        m_visualData = new int[m_cols]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = pow(255.0, float(i) / m_cols);
        }
    }

    short destl[256];
    calc_freq(destl, left);

    const double yscale = (double)1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        short yl = 0;
        int magnitudel = 0;

        if(m_xscale[i] == m_xscale[i + 1])
        {
            yl = (i >= 256 ? 0 : (destl[i] >> 7)); //128
        }

        for(int k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            yl = (k >= 256 ? 0 : qMax(short(destl[k] >> 7), yl));
        }

        if(yl > 0)
        {
            magnitudel = qBound(0, int(log(yl) * yscale), m_rows);
        }

        m_visualData[i] -= m_analyzerSize * m_rows / 15;
        m_visualData[i] = magnitudel > m_visualData[i] ? magnitudel : m_visualData[i];
    }
}
