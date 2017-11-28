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
#include "xrays.h"
#include "colorwidget.h"

XRays::XRays (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = 0;
    m_running = false;
    m_cols = 0;
    m_rows = 0;

    setWindowTitle (tr("XRays"));
    setMinimumSize(2*300-30, 105);
    m_timer = new QTimer (this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));
    m_timer->setInterval(10);

    clear();
    readSettings();
}

XRays::~XRays()
{
    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
}

void XRays::start()
{
    m_running = true;
    if(isVisible())
        m_timer->start();
}

void XRays::stop()
{
    m_running = false;
    m_timer->stop();
    clear();
}

void XRays::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void XRays::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void XRays::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("XRays");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
}

void XRays::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("XRays");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.endGroup();
}

void XRays::changeColor()
{
    ColorWidget c;
    c.setColors(m_colors);
    if(c.exec())
    {
        m_colors = c.getColors();
    }
}

void XRays::hideEvent (QHideEvent *)
{
    m_timer->stop();
}

void XRays::showEvent (QShowEvent *)
{
    if(m_running)
        m_timer->start();
}

void XRays::paintEvent (QPaintEvent * e)
{
    QPainter painter (this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void XRays::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered (QAction *)), SLOT(writeSettings()));
    connect(&menu, SIGNAL(triggered (QAction *)), SLOT(readSettings()));

    menu.addAction("Color", this, SLOT(changeColor()));
    menu.exec(QCursor::pos());
}

void XRays::process ()
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

void XRays::draw (QPainter *p)
{
    QLinearGradient line(0, 0, 0, height());
    for(int i=0; i<m_colors.count(); ++i)
    {
        line.setColorAt((i+1)*1.0/m_colors.count(), m_colors[i]);
    }
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p->setPen(QPen(line, 1));

    float l = 1.0f;
    if(SoundCore::instance())
    {
        l = SoundCore::instance()->volume()*1.0/100;
    }

    for (int i = 0; i<m_cols - 1; ++i)
    {
        int h1 = (m_rows/2 - m_intern_vis_data[i])*l;
        int h2 = (m_rows/2 - m_intern_vis_data[i+1])*l;
        if (h1 > h2)
            qSwap(h1, h2);
        p->drawLine(i, h1, (i+1), h2);
    }
}
