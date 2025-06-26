#include "floridpicture.h"
#include "inlines.h"

#include <QPainter>

FloridPicture::FloridPicture(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Picture Widget"));

    m_enabledLabel = false;
    m_gradientLabel = false;
    m_roundLabel->hide();
}

FloridPicture::~FloridPicture()
{
    delete[] m_xscale;
}

void FloridPicture::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(!m_image.isNull())
    {
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.drawImage(0, 0, m_image.scaled(size()));

        const int h = height() / 5;
        painter.fillRect(0, height() - h, width(), h, QColor(0, 0, 0, 100));
        reRenderLabel();

        for(int i = 0; i < m_cols; ++i)
        {
            const int x = i * m_cellSize.width() + 1;
            const int offset = m_visualData[i] * m_cellSize.height();
            painter.fillRect(x, height() - offset + 1, m_cellSize.width() - 2, offset - 2, m_averageColor);
        }
    }
    else
    {
        painter.fillRect(rect(), Qt::black);
    }
}

void FloridPicture::processData(float *left, float *)
{
    const int rows = (height() / 5 - 2) / m_cellSize.height();
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
            if(i > 0 && m_xscale[i - 1] >= m_xscale[i]) //avoid several bars in a row with the same frequency
            {
                m_xscale[i] = qMin(m_xscale[i - 1] + 1, m_cols);
            }
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
            yl = destl[i] >> 7; //128
        }

        for(int k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            yl = qMax(short(destl[k] >> 7), yl);
        }

        if(yl > 0)
        {
            magnitudel = qBound(0, int(log(yl) * yscale), m_rows);
        }

        m_visualData[i] -= m_analyzerSize * m_rows / 15;
        m_visualData[i] = magnitudel > m_visualData[i] ? magnitudel : m_visualData[i];
    }
}
