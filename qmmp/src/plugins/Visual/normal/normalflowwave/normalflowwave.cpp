#include "normalflowwave.h"
#include "inlines.h"

#include <QMenu>
#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <math.h>
#include <qmmp/qmmp.h>

NormalFlowWave::NormalFlowWave(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Normal FlowWave Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    for(int i = 0; i < 50; ++i)
    {
        m_starPoints << new StarPoint();
    }

    m_starTimer = new QTimer(this);
    connect(m_starTimer, SIGNAL(timeout()), this, SLOT(starTimeout()));

    m_starAction = new QAction(tr("Star"), this);
    m_starAction->setCheckable(true);
    connect(m_starAction, SIGNAL(triggered(bool)), this, SLOT(changeStarState(bool)));

    m_starTimer->setInterval(1000);

    readSettings();
}

NormalFlowWave::~NormalFlowWave()
{
    qDeleteAll(m_starPoints);
    if(m_xscale)
    {
        delete[] m_xscale;
    }
}

void NormalFlowWave::start()
{
    Visual::start();
    if(isVisible())
    {
        m_starTimer->start();
    }
}

void NormalFlowWave::stop()
{
    Visual::stop();
    m_starTimer->stop();
}

void NormalFlowWave::starTimeout()
{
    for(StarPoint *point : qAsConst(m_starPoints))
    {
        point->m_alpha = qrand() % 255;
        point->m_pt = QPoint(qrand() % width(), qrand() % height());
    }
}

void NormalFlowWave::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalFlowWave");
    m_starAction->setChecked(settings.value("show_star", false).toBool());
    m_starColor = ColorWidget::readSingleColorConfig(settings.value("star_color").toString());
    settings.endGroup();
}

void NormalFlowWave::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalFlowWave");
    settings.setValue("show_star", m_starAction->isChecked());
    settings.setValue("star_color", ColorWidget::writeSingleColorConfig(m_starColor));
    settings.endGroup();
}

void NormalFlowWave::changeStarState(bool state)
{
    m_starAction->setChecked(state);
    update();
}

void NormalFlowWave::changeStarColor()
{
    ColorWidget c;
    c.setSingleColorMode(true);
    c.setColor(m_starColor);
    if(c.exec())
    {
        m_starColor = c.color();
        update();
    }
}

void NormalFlowWave::hideEvent(QHideEvent *e)
{
    Visual::hideEvent(e);
    m_starTimer->stop();
}

void NormalFlowWave::showEvent(QShowEvent *e)
{
    Visual::showEvent(e);
    if(m_running)
    {
        m_starTimer->start();
    }
}

void NormalFlowWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setRenderHints(QPainter::Antialiasing);

    if(m_starAction->isChecked())
    {
        for(StarPoint *point : qAsConst(m_starPoints))
        {
            m_starColor.setAlpha(point->m_alpha);
            painter.setPen(QPen(m_starColor, 3));
            painter.drawPoint(point->m_pt);
        }
    }

    QLinearGradient line(0, 0, width(), 0);
    line.setColorAt(1.0 * 1 / 7, QColor(72, 176, 211));
    line.setColorAt(1.0 * 2 / 7, QColor(57, 255, 57));
    line.setColorAt(1.0 * 4 / 7, QColor(255, 247, 22));
    line.setColorAt(1.0 * 5 / 7, QColor(255, 64, 59));
    line.setColorAt(1.0 * 7 / 7, QColor(255, 64, 59));

    for(int i = 0; i < m_cols; ++i)
    {
        const int x = i * m_cellSize.width() + 1;
        for(int j = 0; j <= m_intern_vis_data[i] / 2; ++j)
        {
            painter.fillRect(x, height() / 2 - j * m_cellSize.height() + 1, m_cellSize.width() - 2, m_cellSize.height() - 2, line);
            painter.fillRect(x, height() / 2 + j * m_cellSize.height() + 1, m_cellSize.width() - 2, m_cellSize.height() - 2, line);
        }
    }
    painter.fillRect(0, height() / 2, width(), height() / 2, QColor(0, 0, 0, 188));
}

void NormalFlowWave::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    menu.addAction(m_screenAction);
    menu.addSeparator();
    menu.addAction(m_starAction);
    menu.addAction(tr("StarColor"), this, SLOT(changeStarColor()));
    menu.exec(QCursor::pos());
}

void NormalFlowWave::process(float *left, float *)
{
    const int rows = (height() - 2) / m_cellSize.height();
    const int cols = (width() - 2) / m_cellSize.width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        if(m_xscale)
        {
            delete[] m_xscale;
        }

        m_intern_vis_data = new int[m_cols]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    short y;
    int k, magnitude;

    calc_freq(dest, left);
    const double y_scale = (double) 1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        y = 0;
        magnitude = 0;

        if(m_xscale[i] == m_xscale[i + 1])
        {
            y = dest[i];
        }

        for(k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if(y)
        {
            magnitude = int(log(y) * y_scale);
            magnitude = qBound(0, magnitude, m_rows);
        }

        m_intern_vis_data[i] -= m_analyzerSize * m_rows / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}
