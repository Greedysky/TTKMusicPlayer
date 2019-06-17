#include <QPainter>
#include "fft.h"
#include "inlines.h"
#include "floridreverberation.h"

FloridReverberation::FloridReverberation (QWidget *parent) : Florid (parent)
{
    m_intern_vis_data = nullptr;
    m_x_scale = nullptr;
    m_running = false;
    m_rows = 0;
    m_cols = 0;

    setWindowTitle(tr("Florid Reverberation Widget"));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    m_analyzer_falloff = 1.2;
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);
    m_cell_size = QSize(6, 2);

    clear();
}

FloridReverberation::~FloridReverberation()
{
    if(m_intern_vis_data)
    {
        delete[] m_intern_vis_data;
    }
    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void FloridReverberation::start()
{
    Florid::start();
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
    }
}

void FloridReverberation::stop()
{
    Florid::stop();
    m_running = false;
    m_timer->stop();
    clear();
}

void FloridReverberation::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void FloridReverberation::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void FloridReverberation::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void FloridReverberation::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void FloridReverberation::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);
    QPainter painter(this);
    draw(&painter);
}

void FloridReverberation::process()
{
    static fft_state *state = nullptr;
    if(!state)
    {
        state = fft_init();
    }

    const int rows = (height() - 2) / m_cell_size.height();
    const int cols = (width() - 2) / m_cell_size.width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }
        if(m_x_scale)
        {
            delete[] m_x_scale;
        }
        m_intern_vis_data = new double[m_cols];
        m_x_scale = new int[m_cols + 1];

        for(int i = 0; i < m_cols; ++i)
        {
            m_intern_vis_data[i] = 0;
        }
        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    short y;
    int k, magnitude;

    calc_freq (dest, m_left_buffer);

    const double y_scale = (double) 1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; i++)
    {
        y = 0;
        magnitude = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            y = dest[i];
        }
        for(k = m_x_scale[i]; k < m_x_scale[i + 1]; k++)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if(y)
        {
            magnitude = int(log (y) * y_scale);
            magnitude = qBound(0, magnitude, m_rows);
        }

        m_intern_vis_data[i] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}

void FloridReverberation::draw(QPainter *p)
{
    if(m_cols == 0)
    {
        return;
    }

    p->setRenderHints(QPainter::Antialiasing);
    p->setPen(QPen(QColor(64, 229, 255), 3));
    p->translate(rect().center());

    qreal startAngle = 45;
    for(int i = 0; i < DISTANCE; ++i)
    {
        p->save();
        p->rotate(startAngle);
        QPointF bottomPot(0, DISTANCE);
        QPointF topPot(0, DISTANCE + m_intern_vis_data[int(i * m_cols * 1.0 / DISTANCE)] *0.5);
        p->drawLine(bottomPot, topPot);
        p->restore();
        startAngle += 3.6;
    }
}
