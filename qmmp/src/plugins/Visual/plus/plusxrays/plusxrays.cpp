#include "plusxrays.h"
#include "colorwidget.h"

#include <QMenu>
#include <QSettings>
#include <QPainter>
#include <qmmp/qmmp.h>

PlusXRays::PlusXRays(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Plus XRays Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    m_gridAction = new QAction(tr("Grid"), this);
    m_gridAction->setCheckable(true);

    readSettings();
}

void PlusXRays::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusXRays");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    m_gridAction->setChecked(settings.value("show_grid", false).toBool());
    settings.endGroup();
}

void PlusXRays::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusXRays");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.setValue("show_grid", m_gridAction->isChecked());
    settings.endGroup();
}

void PlusXRays::changeColor()
{
    ColorWidget dialog;
    dialog.setColors(m_colors);
    if(dialog.exec())
    {
        m_colors = dialog.colors();
    }
}

void PlusXRays::changeGridState(bool state)
{
    m_gridAction->setChecked(state);
    update();
}

void PlusXRays::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);

    if(m_gridAction->isChecked())
    {
        QLinearGradient line(0, 0, 0, height());
        for(int i = 0; i < m_colors.count(); ++i)
        {
            QColor color = m_colors[i];
            color.setAlpha(50);
            line.setColorAt((i + 1) * 1.0 / m_colors.count(), color);
        }
        painter.setPen(QPen(line, 1));

        int per = width() / 8;

        for(int w = 0; w < width(); ++w)
        {
            painter.drawLine(QPoint(w * per, 0), QPoint(w * per, height()));
        }

        per = height() / 8;
        for(int h = 0; h < height(); ++h)
        {
            painter.drawLine(QPoint(0, h * per), QPoint(width(), h * per));
        }
    }

    QLinearGradient line(0, 0, 0, height());
    for(int i = 0; i < m_colors.count(); ++i)
    {
        line.setColorAt((i + 1) * 1.0 / m_colors.count(), m_colors[i]);
    }
    painter.setPen(QPen(line, 1));

    for(int i = 0; i < m_cols; ++i)
    {
        if(i + 1 >= m_cols)
        {
            break;
        }

        int front = m_rows / 2 - m_visualData[i];
        int end = m_rows / 2 - m_visualData[i + 1];

        if(front > end)
        {
            qSwap(front, end);
        }

        painter.drawLine(i, front, i + 1, end);
    }
}

void PlusXRays::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    menu.addAction(m_screenAction);
    menu.addSeparator();

    menu.addAction(tr("Color"), this, SLOT(changeColor()));
    menu.addAction(m_gridAction);
    menu.exec(QCursor::pos());
}

void PlusXRays::processData(float *left, float *)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_visualData;
        m_visualData = new int[m_cols]{0};
    }

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols; ++i)
    {
        pos += step;
        m_visualData[i] = int(left[pos >> 8] * m_rows / 2);
        m_visualData[i] = qBound(-m_rows / 2, m_visualData[i], m_rows / 2);
    }
}
