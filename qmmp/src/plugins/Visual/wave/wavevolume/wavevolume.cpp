#include "wavevolume.h"
#include "inlines.h"

#include <QMenu>
#include <QPainter>
#include <math.h>

WaveVolume::WaveVolume(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Wave Volume Widget"));
    setMinimumSize(2 * 300 - 30, 105);
}

WaveVolume::~WaveVolume()
{
    delete[] m_xscale;
}

void WaveVolume::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setRenderHints(QPainter::Antialiasing);

    QLinearGradient line(0, 0, width(), 0);
    line.setColorAt(0.0f, QColor(0, 0xff, 0));
    line.setColorAt(0.65f, QColor(0xff, 0xff, 0));
    line.setColorAt(1.0f, QColor(0xff, 0, 0));
    painter.fillRect(0, 0, width(), height(), line);
    painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 88));

    line.setColorAt(0.0f, QColor(0, 0xff, 0));
    line.setColorAt(0.65f, QColor(0xff, 0xff, 0));
    line.setColorAt(1.0f, QColor(0xff, 0, 0));

    if(m_visualData)
    {
        const int wid = ceil(m_rows / 2);
        painter.fillRect(0, 0, m_visualData[0] * m_cols / m_rows, wid, line);
        painter.fillRect(0, wid, m_visualData[1] * m_cols / m_rows, wid, line);
    }

    painter.setPen(Qt::white);
    painter.drawText(10, height() / 4, "L");
    painter.drawText(10, height() * 3 / 4, "R");
}

void WaveVolume::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}

void WaveVolume::processData(float *left, float *right)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_visualData;
        delete[] m_xscale;

        m_visualData = new int[2]{0};
        m_xscale = new int[2]{0};

        for(int i = 0; i < 2; ++i)
        {
            m_xscale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short destl[256], destr[256];
    calc_freq(destl, left);
    calc_freq(destr, right);

    short yl = 0, yr = 0;
    int magnitudel = 0, magnituder = 0;
    const double yscale = (double)1.25 * m_rows / log(256);

    if(m_xscale[0] == m_xscale[1])
    {
        yl = destl[0];
        yr = destr[0];
    }

    for(int k = m_xscale[0]; k < m_xscale[1]; ++k)
    {
        yl = qMax(destl[k], yl);
        yr = qMax(destr[k], yr);
    }

    yl >>= 7; //256
    yr >>= 7;

    if(yl)
    {
        magnitudel = int(log(yl) * yscale);
        magnitudel = qBound(0, magnitudel, m_rows);
    }

    if(yr)
    {
        magnituder = int(log(yr) * yscale);
        magnituder = qBound(0, magnituder, m_rows);
    }

    m_visualData[0] -= m_analyzerSize * m_rows / 15;
    m_visualData[0] = magnitudel > m_visualData[0] ? magnitudel : m_visualData[0];

    m_visualData[1] -= m_analyzerSize * m_rows / 15;
    m_visualData[1] = magnituder > m_visualData[1] ? magnituder : m_visualData[1];

}
