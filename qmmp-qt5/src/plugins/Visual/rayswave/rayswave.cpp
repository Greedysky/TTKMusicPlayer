#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <qmmp/soundcore.h>
#include "fft.h"
#include "inlines.h"
#include "rayswave.h"

RaysWave::RaysWave (QWidget *parent) : Visual (parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setAttribute(Qt::WA_QuitOnClose, false);

    m_intern_vis_data = 0;
    m_running = false;
    m_cols = 0;
    m_rows = 0;

    setWindowTitle (tr("RaysWave"));
    m_timer = new QTimer (this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));
    m_timer->setInterval(10);

    clear();
}

RaysWave::~RaysWave()
{
    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
}

void RaysWave::start()
{
    m_running = true;
    if(isVisible())
        m_timer->start();
}

void RaysWave::stop()
{
    m_running = false;
    m_timer->stop();
    clear();
}

void RaysWave::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void RaysWave::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void RaysWave::hideEvent (QHideEvent *)
{
    m_timer->stop();
}

void RaysWave::showEvent (QShowEvent *)
{
    if(m_running)
        m_timer->start();
}

void RaysWave::paintEvent (QPaintEvent * e)
{
    Q_UNUSED(e);
    QPainter painter (this);
    draw(&painter);
}

void RaysWave::process ()
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

void RaysWave::draw (QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p->setPen(QPen(Qt::white, 1));

    float l = 1.0f;
    if(SoundCore::instance())
    {
        l = SoundCore::instance()->volume()*1.0/100;
    }

    for (int i = 0; i<m_cols; ++i)
    {
        if((i+1) >= m_cols)
            break;

        int h1 = m_rows/2 - m_intern_vis_data[i]*l;
        int h2 = m_rows/2 - m_intern_vis_data[i+1]*l;
        if (h1 > h2)
            qSwap(h1, h2);
        p->drawLine(i, h1, (i+1), h2);
    }
}
