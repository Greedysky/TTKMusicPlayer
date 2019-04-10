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
#include "plusxrays.h"
#include "colorwidget.h"

PlusXRays::PlusXRays (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = nullptr;
    m_running = false;
    m_cols = 0;
    m_rows = 0;

    setWindowTitle(tr("Plus XRays Widget"));
    setMinimumSize(2*300-30, 105);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);

    m_gridAction = new QAction(tr("Grid"), this);
    m_gridAction->setCheckable(true);
    connect(m_gridAction, SIGNAL(triggered(bool)), this, SLOT(changeGridState(bool)));

    clear();
    readSettings();
}

PlusXRays::~PlusXRays()
{
    if(m_intern_vis_data)
        delete[] m_intern_vis_data;
}

void PlusXRays::start()
{
    m_running = true;
    if(isVisible())
        m_timer->start();
}

void PlusXRays::stop()
{
    m_running = false;
    m_timer->stop();
    clear();
}

void PlusXRays::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void PlusXRays::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
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
    ColorWidget c;
    c.setColors(m_colors);
    if(c.exec())
    {
        m_colors = c.getColors();
    }
}

void PlusXRays::changeGridState(bool state)
{
    m_gridAction->setChecked(state);
    update();
}

void PlusXRays::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void PlusXRays::showEvent(QShowEvent *)
{
    if(m_running)
        m_timer->start();
}

void PlusXRays::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void PlusXRays::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(readSettings()));

    menu.addAction("Color", this, SLOT(changeColor()));
    menu.addAction(m_gridAction);

    menu.exec(QCursor::pos());
}

void PlusXRays::process()
{
    static fft_state *state = nullptr;
    if(!state)
        state = fft_init();

    m_cols = width();
    m_rows = height();

    if(m_intern_vis_data)
        delete[] m_intern_vis_data;

    m_intern_vis_data = new int[m_cols];

    const int step = (QMMP_VISUAL_NODE_SIZE << 8)/m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(m_left_buffer[pos >> 8] * m_rows* 1.0);
        m_intern_vis_data[i] = qBound(-m_rows/2, m_intern_vis_data[i], m_rows/2);
    }
}

void PlusXRays::draw(QPainter *p)
{
    if(m_gridAction->isChecked())
    {
        p->setPen(QPen(QColor(255, 255, 255, 50), 1));
        int per = width()/8;
        for(int w=0; w<width(); ++w)
        {
            p->drawLine(QPoint(w*per, 0), QPoint(w*per, height()));
        }

        per = height()/8;
        for(int h=0; h<height(); ++h)
        {
            p->drawLine(QPoint(0, h*per), QPoint(width(), h*per));
        }
    }

    QLinearGradient line(0, 0, 0, height());
    for(int i=0; i<m_colors.count(); ++i)
    {
        line.setColorAt((i+1)*1.0/m_colors.count(), m_colors[i]);
    }
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p->setPen(QPen(line, 1));

    const float maxed = maxRange();

    for(int i = 0; i<m_cols; ++i)
    {
        if((i+1) >= m_cols)
        {
            break;
        }

        int h1 = m_rows/2 - m_intern_vis_data[i] * maxed;
        int h2 = m_rows/2 - m_intern_vis_data[i + 1] * maxed;
        if(h1 > h2)
        {
            qSwap(h1, h2);
        }
        p->drawLine(i, h1, (i+1), h2);
    }
}
