#include "normalhistogram.h"
#include "inlines.h"

#include <QMenu>
#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <cmath>
#include <qmmp/qmmp.h>

NormalHistogram::NormalHistogram(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Normal Histogram Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    readSettings();
}

NormalHistogram::~NormalHistogram()
{
    delete[] m_xscale;
}

void NormalHistogram::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalHistogram");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    settings.endGroup();
}

void NormalHistogram::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalHistogram");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.endGroup();
}

void NormalHistogram::changeColor()
{
    ColorWidget dialog;
    dialog.setColors(m_colors);
    if(dialog.exec())
    {
        m_colors = dialog.colors();
    }
}

void NormalHistogram::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setRenderHints(QPainter::Antialiasing);

    QLinearGradient line(0, 0, 0, height());
    for(int i = 0; i < m_colors.count(); ++i)
    {
        line.setColorAt((i + 1) * 1.0 / m_colors.count(), m_colors[i]);
    }

    for(int i = 0; i < m_cols; ++i)
    {
        const int x = i * m_cellSize.width() + 1;
        const int offset = m_visualData[i] * m_cellSize.height();
        painter.fillRect(x, height() - offset + 1, m_cellSize.width() - 2, offset - 2, line);
    }
}

void NormalHistogram::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    menu.addAction(m_screenAction);
    menu.addSeparator();

    menu.addAction(tr("Color"), this, SLOT(changeColor()));
    menu.exec(QCursor::pos());
}

void NormalHistogram::processData(float *left, float *)
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
