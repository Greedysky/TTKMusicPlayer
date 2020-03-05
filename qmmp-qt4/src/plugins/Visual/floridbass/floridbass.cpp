#include <QPainter>
#include "fft.h"
#include "inlines.h"
#include "floridbass.h"

FloridBass::FloridBass (QWidget *parent)
    : Florid(parent)
{
    m_gradientOn = true;
    m_intern_vis_data = nullptr;
    m_running = false;
    m_rows = 0;
    m_cols = 0;

    setWindowTitle(tr("Florid Bass Widget"));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    m_timer->setInterval(QMMP_VISUAL_INTERVAL * 1.5);

    clear();
}

FloridBass::~FloridBass()
{
    if(m_intern_vis_data)
    {
        delete[] m_intern_vis_data;
    }
}

void FloridBass::start()
{
    Florid::start();
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
    }
}

void FloridBass::stop()
{
    Florid::stop();
    m_running = false;
    m_timer->stop();
    clear();
}

void FloridBass::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void FloridBass::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        Florid::start();
        process();
        update();
    }
    else
    {
        Florid::stop();
    }
}

void FloridBass::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void FloridBass::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void FloridBass::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);
    QPainter painter(this);
    draw(&painter);
}

void FloridBass::process()
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

    m_intern_vis_data = new int[m_cols * 2];

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols * 2; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(m_left_buffer[pos >> 8] * m_rows* 1.0);
        m_intern_vis_data[i] = qBound(-m_rows / 2, m_intern_vis_data[i], m_rows / 2);
        m_intern_vis_data[m_cols * 2 - i - 1] = m_intern_vis_data[i];
    }
}

void FloridBass::draw(QPainter *p)
{
    if(m_cols == 0)
    {
        return;
    }

    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p->setPen(QPen(m_averageColor, 2));
    p->translate(rect().center());

    qreal startAngle = 0;
    for(int i = 0; i < m_rows * 2; ++i)
    {
        p->save();
        p->rotate(startAngle);
        int value1 = m_intern_vis_data[i];
        int value2 = m_intern_vis_data[i + 1];
        if(value1 > value2)
        {
            qSwap(value1, value2);
        }

        p->drawLine(0, DISTANCE + 10 + value1 * 0.03, 0, DISTANCE + 10 + value2 * 0.03);

        if(value1 < 0)
        {
           value1 = 0;
        }
        p->drawPoint(0, DISTANCE + 20 + value1);

        p->restore();
        startAngle += 360.0 / (m_rows * 2);
    }
}
