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
            m_xscale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    calc_freq(dest, left);

    const double yscale = (double)1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        short y = 0;
        int magnitude = 0;

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
