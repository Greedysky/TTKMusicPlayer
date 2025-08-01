#include "normalline.h"
#include "inlines.h"

#include <QMenu>
#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <math.h>
#include <qmmp/qmmp.h>

NormalLine::NormalLine(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Normal Line Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    readSettings();
}

NormalLine::~NormalLine()
{
    delete[] m_peaks;
    delete[] m_xscale;
}

void NormalLine::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalLine");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    settings.endGroup();
}

void NormalLine::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalLine");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.endGroup();
}

void NormalLine::changeColor()
{
    ColorWidget dialog;
    dialog.setColors(m_colors);
    if(dialog.exec())
    {
        m_colors = dialog.colors();
    }
}

void NormalLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setRenderHints(QPainter::Antialiasing);

    QLinearGradient line(0, 0, 0, height());
    for(int i = 0; i < m_colors.count(); ++i)
    {
        line.setColorAt((i + 1) * 1.0 / m_colors.count(), m_colors[i]);
    }

    const int rdx = qMax(0, width() - 2 * m_cellSize.width() * m_cols);

    for(int i = 0; i < m_cols * 2; ++i)
    {
        int x = i * m_cellSize.width() + 1;
        if(i >= m_cols)
        {
            x += rdx; //correct right part position
        }

        const int offset = m_visualData[i] * m_cellSize.height();
        painter.fillRect(x, height() - offset, m_cellSize.width() - 1, offset, line);
        painter.fillRect(x, height() - int(m_peaks[i]) * m_cellSize.height(), m_cellSize.width() - 1, m_cellSize.height(), "Cyan");
    }
}

void NormalLine::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    menu.addAction(tr("Color"), this, SLOT(changeColor()));
    menu.exec(QCursor::pos());
}

void NormalLine::processData(float *left, float *right)
{
    const int rows = (height() - 2) / m_cellSize.height();
    const int cols = (width() - 2) / m_cellSize.width() / 2;

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_peaks;
        delete[] m_visualData;
        delete[] m_xscale;

        m_peaks = new double[m_cols * 2]{0};
        m_visualData = new int[m_cols * 2]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = pow(255.0, float(i) / m_cols);
        }
    }

    short destl[256], destr[256];
    calc_freq(destl, left);
    calc_freq(destr, right);

    const double yscale = (double)1.25 * m_rows / log(256);

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
            magnitudel = qBound(0, int(log(yl) * yscale), m_rows);
        }

        if(yr > 0)
        {
            magnituder = qBound(0, int(log(yr) * yscale), m_rows);
        }

        m_visualData[i] -= m_analyzerSize * m_rows / 15;
        m_visualData[i] = magnitudel > m_visualData[i] ? magnitudel : m_visualData[i];

        const int j = m_cols * 2 - i - 1; //mirror index
        m_visualData[j] -= m_analyzerSize * m_rows / 15;
        m_visualData[j] = magnituder > m_visualData[j] ? magnituder : m_visualData[j];

        m_peaks[i] -= m_peakSize * m_rows / 15;
        m_peaks[i] = magnitudel > m_peaks[i] ? magnitudel : m_peaks[i];

        m_peaks[j] -= m_peakSize * m_rows / 15;
        m_peaks[j] = magnituder > m_peaks[j] ? magnituder : m_peaks[j];
    }
}
