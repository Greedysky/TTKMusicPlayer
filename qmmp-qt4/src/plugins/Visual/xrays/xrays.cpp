#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QActionGroup>
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

#define VISUAL_NODE_SIZE 512 //samples
#define VISUAL_BUFFER_SIZE (5*VISUAL_NODE_SIZE)

XRays::XRays (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = 0;
    m_buffer_at = 0;
    m_cols = 0;
    m_rows = 0;

    setWindowTitle (tr("XRays"));
    setMinimumSize(2*300-30, 105);
    m_timer = new QTimer (this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));
    m_buffer = new float[VISUAL_BUFFER_SIZE];
    m_timer->setInterval(10);

    clear();
    createMenu();
    readSettings();
}

XRays::~XRays()
{
    delete [] m_buffer;
    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
}

void XRays::add (float *data, size_t samples, int chan)
{
    if (!m_timer->isActive ())
        return;

    if(VISUAL_BUFFER_SIZE == m_buffer_at)
    {
        m_buffer_at -= VISUAL_NODE_SIZE;
        memmove(m_buffer, m_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
        return;
    }

    int frames = qMin(int(samples/chan), VISUAL_BUFFER_SIZE - m_buffer_at);
    mono_from_multichannel(m_buffer + m_buffer_at, data, frames, chan);

    m_buffer_at += frames;
}

void XRays::clear()
{
    m_buffer_at = 0;
    m_cols = 0;
    update();
}

void XRays::timeout()
{
    mutex()->lock();
    if(m_buffer_at < VISUAL_NODE_SIZE)
    {
        mutex()->unlock ();
        return;
    }

    process (m_buffer);
    m_buffer_at -= VISUAL_NODE_SIZE;
    memmove(m_buffer, m_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
    mutex()->unlock ();
    update();
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
    m_timer->start();
}

void XRays::paintEvent (QPaintEvent * e)
{
    QPainter painter (this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void XRays::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        m_menu->exec(e->globalPos());
}

void XRays::process (float *buffer)
{
    m_cols = width();
    m_rows = height();

    if(m_intern_vis_data)
        delete [] m_intern_vis_data;

    m_intern_vis_data = new int[m_cols];

    int step = (VISUAL_NODE_SIZE << 8)/m_cols;
    int pos = 0;

    for (int i = 0; i < m_cols; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(buffer[pos >> 8] * m_rows* 1.0);
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

void XRays::createMenu()
{
    m_menu = new QMenu (this);
    connect(m_menu, SIGNAL(triggered (QAction *)),SLOT(writeSettings()));
    connect(m_menu, SIGNAL(triggered (QAction *)),SLOT(readSettings()));

    m_menu->addAction("Color", this, SLOT(changeColor()));

    update();
}
