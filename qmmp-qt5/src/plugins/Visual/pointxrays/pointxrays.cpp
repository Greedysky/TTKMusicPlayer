#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <qmmp/soundcore.h>
#include "fft.h"
#include "inlines.h"
#include "pointxrays.h"
#include "colorwidget.h"

PointXRays::PointXRays (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = 0;
    m_running = false;
    m_cols = 0;
    m_rows = 0;

    setWindowTitle (tr("PointXRays"));
    setMinimumSize(2*300-30, 105);
    m_timer = new QTimer (this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));
    m_timer->setInterval(10);

    m_gridAction = new QAction(tr("Grid"), this);
    m_gridAction->setCheckable(true);
    connect(m_gridAction, SIGNAL(triggered(bool)), this, SLOT(changeGridState(bool)));

    clear();
    readSettings();
}

PointXRays::~PointXRays()
{
    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
}

void PointXRays::start()
{
    m_running = true;
    if(isVisible())
        m_timer->start();
}

void PointXRays::stop()
{
    m_running = false;
    m_timer->stop();
    clear();
}

void PointXRays::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void PointXRays::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void PointXRays::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PointXRays");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    m_gridAction->setChecked(settings.value("show_grid", false).toBool());
}

void PointXRays::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PointXRays");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.setValue("show_grid", m_gridAction->isChecked());
    settings.endGroup();
}

void PointXRays::changeColor()
{
    ColorWidget c;
    c.setColors(m_colors);
    if(c.exec())
    {
        m_colors = c.getColors();
    }
}

void PointXRays::changeGridState(bool state)
{
    m_gridAction->setChecked(state);
    update();
}

void PointXRays::hideEvent (QHideEvent *)
{
    m_timer->stop();
}

void PointXRays::showEvent (QShowEvent *)
{
    if(m_running)
        m_timer->start();
}

void PointXRays::paintEvent (QPaintEvent * e)
{
    QPainter painter (this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void PointXRays::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered (QAction *)), SLOT(writeSettings()));
    connect(&menu, SIGNAL(triggered (QAction *)), SLOT(readSettings()));

    menu.addAction("Color", this, SLOT(changeColor()));
    menu.addAction(m_gridAction);

    menu.exec(QCursor::pos());
}

void PointXRays::process ()
{
    static fft_state *state = 0;
    if (!state)
        state = fft_init();

    m_cols = width();
    m_rows = height();

    if(m_intern_vis_data)
        delete [] m_intern_vis_data;

    m_intern_vis_data = new int[m_cols];

    int step = (QMMP_VISUAL_NODE_SIZE << 8)/m_cols;
    int pos = 0;

    for (int i = 0; i < m_cols; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(m_left_buffer[pos >> 8] * m_rows* 1.0);
        m_intern_vis_data[i] = qBound(-m_rows/2, m_intern_vis_data[i], m_rows/2);
    }
}

void PointXRays::draw (QPainter *p)
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
    p->setPen(QPen(line, 3));

    float l = 1.0f;
    if(SoundCore::instance())
    {
        l = SoundCore::instance()->volume()*1.0/100;
    }

    for (int i = 0; i<m_cols; ++i)
    {
        int v = m_intern_vis_data[i]*l;
        if(v != 0 && i%5 == 0)
        {
            p->drawPoint(i, m_rows/2 - m_intern_vis_data[i]*l);
        }
    }
}
