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
    delete[] m_xscale;
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
        const double value = m_visualData[int(i * m_cols * 0.8 / LABEL_RADIUS)];
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

        delete[] m_visualData;
        delete[] m_xscale;

        m_visualData = new int[m_cols]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = std::pow(255.0, float(i) / m_cols);
        }
    }

    short destl[256];
    calc_freq(destl, left);

    const double yscale = (double)1.25 * m_rows / std::log(256);

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
            magnitudel = qBound(0, int(std::log(yl) * yscale), m_rows);
        }

        m_visualData[i] -= m_analyzerSize * m_rows / 15;
        m_visualData[i] = magnitudel > m_visualData[i] ? magnitudel : m_visualData[i];
    }
}
