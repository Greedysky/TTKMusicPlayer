#include "plusfoldwave.h"
#include "inlines.h"

#include <QMenu>
#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <cmath>
#include <qmmp/qmmp.h>

PlusFoldWave::PlusFoldWave(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Plus FoldWave Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    readSettings();
}

PlusFoldWave::~PlusFoldWave()
{
    delete[] m_xscale;
}

void PlusFoldWave::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusFoldWave");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    settings.endGroup();
}

void PlusFoldWave::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusFoldWave");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.endGroup();
}

void PlusFoldWave::changeColor()
{
    ColorWidget dialog;
    dialog.setColors(m_colors);
    if(dialog.exec())
    {
        m_colors = dialog.colors();
    }
}

void PlusFoldWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);

    QLinearGradient line(0, 0, 0, height());
    for(int i = 0; i < m_colors.count(); ++i)
    {
        line.setColorAt((i + 1) * 1.0 / m_colors.count(), m_colors[i]);
    }

    painter.setPen(QPen(line, 1));

    const int rdx = qMax(0, width() - 2 * m_cellSize.width() * m_cols);

    for(int i = 0; i < m_cols * 2; ++i)
    {
        int x = i * m_cellSize.width() + 1;
        if(i >= m_cols)
        {
            x += rdx; //correct right part position
        }

        int offset = m_visualData[i] * m_cellSize.height() / 2;
        if(std::abs(offset) > height() / 2)
        {
            offset = height() / 2;
        }

        painter.drawLine(QPoint(x, height() / 2 - offset), QPoint(x, height() / 2 + offset));

        if(i + 1 >= m_cols * 2)
        {
            break;
        }

        int x1 = (i + 1) * m_cellSize.width() + 1;
        if(i + 1 >= m_cols)
        {
            x1 += rdx; //correct right part position
        }

        int offset1 = m_visualData[i + 1] * m_cellSize.height() / 2;
        if(std::abs(offset1) > height() / 2)
        {
            offset1 = height() / 2;
        }

        painter.drawLine(QPoint(x, height() / 2 - offset), QPoint(x1, height() / 2 - offset1));
        painter.drawLine(QPoint(x, height() / 2 + offset), QPoint(x1, height() / 2 + offset1));
    }
}

void PlusFoldWave::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    menu.addAction(tr("Color"), this, SLOT(changeColor()));
    menu.exec(QCursor::pos());
}

void PlusFoldWave::processData(float *left, float *right)
{
    const int rows = (height() - 2) / m_cellSize.height();
    const int cols = (width() - 2) / m_cellSize.width() / 2;

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_visualData;
        delete[] m_xscale;

        m_visualData = new int[m_cols * 2]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = std::pow(255.0, float(i) / m_cols);
        }
    }

    short destl[256], destr[256];
    calc_freq(destl, left);
    calc_freq(destr, right);

    const double yscale = (double)1.25 * m_rows / std::log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        short yl = 0, yr = 0;
        int magnitudel = 0, magnituder = 0;

        if(m_xscale[i] == m_xscale[i + 1])
        {
            yl = (i >= 256 ? 0 : (destl[i] >> 7)); //128
            yr = (i >= 256 ? 0 : (destr[i] >> 7)); //128
        }

        for(int k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            yl = (k >= 256 ? 0 : qMax(short(destl[k] >> 7), yl));
            yr = (k >= 256 ? 0 : qMax(short(destr[k] >> 7), yr));
        }

        if(yl > 0)
        {
            magnitudel = qBound(0, int(std::log(yl) * yscale), m_rows);
        }

        if(yr > 0)
        {
            magnituder = qBound(0, int(std::log(yr) * yscale), m_rows);
        }

        m_visualData[i] -= m_analyzerSize * m_rows / 15;
        m_visualData[i] = magnitudel > m_visualData[i] ? magnitudel : m_visualData[i];

        const int j = m_cols * 2 - i - 1; //mirror index
        m_visualData[j] -= m_analyzerSize * m_rows / 15;
        m_visualData[j] = magnituder > m_visualData[j] ? magnituder : m_visualData[j];
    }
}
