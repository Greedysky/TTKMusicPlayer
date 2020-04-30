#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>
#include <qmmp/qmmp.h>

#include "fft.h"
#include "inlines.h"
#include "pluspointxrays.h"
#include "colorwidget.h"

PlusPointXRays::PlusPointXRays (QWidget *parent)
    : Visual(parent)
{
    m_intern_vis_data = nullptr;
    m_running = false;
    m_cols = 0;
    m_rows = 0;

    setWindowTitle(tr("Plus PointXRays Widget"));
    setMinimumSize(2*300-30, 105);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);

    m_screenAction = new QAction(tr("Fullscreen"), this);
    m_screenAction->setCheckable(true);
    connect(m_screenAction, SIGNAL(triggered(bool)), this, SLOT(changeFullScreen(bool)));

    m_gridAction = new QAction(tr("Grid"), this);
    m_gridAction->setCheckable(true);
    connect(m_gridAction, SIGNAL(triggered(bool)), this, SLOT(changeGridState(bool)));

    clear();
    readSettings();
}

PlusPointXRays::~PlusPointXRays()
{
    if(m_intern_vis_data)
    {
        delete[] m_intern_vis_data;
    }
}

void PlusPointXRays::start()
{
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
    }
}

void PlusPointXRays::stop()
{
    m_running = false;
    m_timer->stop();
    clear();
}

void PlusPointXRays::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void PlusPointXRays::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void PlusPointXRays::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusPointXRays");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    m_gridAction->setChecked(settings.value("show_grid", false).toBool());
    settings.endGroup();
}

void PlusPointXRays::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusPointXRays");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.setValue("show_grid", m_gridAction->isChecked());
    settings.endGroup();
}

void PlusPointXRays::changeColor()
{
    ColorWidget c;
    c.setColors(m_colors);
    if(c.exec())
    {
        m_colors = c.getColors();
    }
}

void PlusPointXRays::changeGridState(bool state)
{
    m_gridAction->setChecked(state);
    update();
}

void PlusPointXRays::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void PlusPointXRays::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void PlusPointXRays::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void PlusPointXRays::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(readSettings()));

    menu.addAction(m_screenAction);
    menu.addSeparator();
    menu.addAction(tr("Color"), this, SLOT(changeColor()));
    menu.addAction(m_gridAction);
    menu.exec(QCursor::pos());
}

void PlusPointXRays::process()
{
    static fft_state *state = nullptr;
    if(!state)
    {
        state = fft_init();
    }

    m_cols = width();
    m_rows = height();

    if(m_intern_vis_data)
    {
        delete[] m_intern_vis_data;
    }

    m_intern_vis_data = new int[m_cols];

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(m_left_buffer[pos >> 8] * m_rows * 1.0);
        m_intern_vis_data[i] = qBound(-m_rows / 2, m_intern_vis_data[i], m_rows / 2);
    }
}

void PlusPointXRays::draw(QPainter *p)
{
    if(m_gridAction->isChecked())
    {
        p->setPen(QPen(QColor(255, 255, 255, 50), 1));
        int per = width() / 8;
        for(int w=0; w<width(); ++w)
        {
            p->drawLine(QPoint(w*per, 0), QPoint(w*per, height()));
        }

        per = height() / 8;
        for(int h=0; h<height(); ++h)
        {
            p->drawLine(QPoint(0, h*per), QPoint(width(), h*per));
        }
    }

    QLinearGradient line(0, 0, 0, height());
    for(int i = 0; i < m_colors.count(); ++i)
    {
        line.setColorAt((i + 1) * 1.0 / m_colors.count(), m_colors[i]);
    }
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p->setPen(QPen(line, 3));

    const float maxed = takeMaxRange();

    for(int i = 0; i < m_cols; ++i)
    {
        const int v = m_intern_vis_data[i] * maxed;
        if(v != 0 && i % 5 == 0)
        {
            p->drawPoint(i, m_rows / 2 - m_intern_vis_data[i] * maxed);
        }
    }
}
