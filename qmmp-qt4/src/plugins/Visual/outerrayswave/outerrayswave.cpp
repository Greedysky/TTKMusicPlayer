#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>

#include <qmmp/qmmp.h>
#include "fft.h"
#include "inlines.h"
#include "outerrayswave.h"

OuterRaysWave::OuterRaysWave (QWidget *parent)
    : Visual(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setAttribute(Qt::WA_QuitOnClose, false);

    m_intern_vis_data = nullptr;
    m_running = false;
    m_cols = 0;
    m_rows = 0;

    setWindowTitle(tr("Outer RaysWave Widget"));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);

    clear();
}

OuterRaysWave::~OuterRaysWave()
{
    if(m_intern_vis_data)
    {
        delete[] m_intern_vis_data;
    }
}

void OuterRaysWave::start()
{
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
    }
}

void OuterRaysWave::stop()
{
    m_running = false;
    m_timer->stop();
    clear();
}

void OuterRaysWave::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void OuterRaysWave::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void OuterRaysWave::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void OuterRaysWave::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void OuterRaysWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    draw(&painter);
}

void OuterRaysWave::process()
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
        m_intern_vis_data[i] = int(m_left_buffer[pos >> 8] * m_rows* 1.0);
        m_intern_vis_data[i] = qBound(-m_rows / 2, m_intern_vis_data[i], m_rows / 2);
    }
}

void OuterRaysWave::draw(QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p->setPen(QPen(Qt::white, 1));

    const float maxed = takeMaxRange();

    for(int i = 0; i < m_cols; ++i)
    {
        if((i + 1) >= m_cols)
        {
            break;
        }

        int pFront = m_rows / 2 - m_intern_vis_data[i] * maxed;
        int pEnd = m_rows / 2 - m_intern_vis_data[i + 1] * maxed;
        if(pFront > pEnd)
        {
            qSwap(pFront, pEnd);
        }
        p->drawLine(i, pFront, i + 1, pEnd);
    }
}
