#include <QPainter>
#include "fft.h"
#include "inlines.h"
#include "floridethereality.h"
#include "ethereality.h"

FloridEthereality::FloridEthereality (QWidget *parent)
    : Florid(parent)
{
    m_useImage = false;
    m_intern_vis_data = nullptr;
    m_running = false;
    m_rows = 0;
    m_cols = 0;
    m_pos_x = 0;
    m_pos_y = 0;

    qsrand(QDateTime::currentMSecsSinceEpoch());
    setWindowTitle(tr("Florid Ethereality Widget"));

    for(int i = 0; i < 50; i++)
    {
        Ethereality *ethereality = new Ethereality(this);
        ethereality->resize(20, 200);
        ethereality->move(i * 20, 0);
        m_etherealitys << ethereality;
    }

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);
}

FloridEthereality::~FloridEthereality()
{
    if(m_intern_vis_data)
    {
        delete[] m_intern_vis_data;
    }
    qDeleteAll(m_etherealitys);
}

void FloridEthereality::start()
{
    Florid::start();
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
        foreach(Ethereality *ethereality, m_etherealitys)
        {
            ethereality->start();
        }
    }
}

void FloridEthereality::stop()
{
    Florid::stop();
    m_running = false;
    m_timer->stop();
    foreach(Ethereality *ethereality, m_etherealitys)
    {
        ethereality->stop();
    }
    clear();
}

void FloridEthereality::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void FloridEthereality::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        Florid::start();
        process();

        if(m_cols == 0)
        {
            return;
        }

        int max = 0;
        for(int i = 0; i < m_rows * 2; ++i)
        {
            const int value = std::abs(m_intern_vis_data[i]);
            if(max < value)
            {
                max = value;
            }
        }
        max = (max <= 3) ? 10 : max / 3;
        foreach(Ethereality *ethereality, m_etherealitys)
        {
            if(ethereality->isRunning())
            {
                m_pos_x = ethereality->pos().x();
                m_pos_y = ethereality->pos().y();
                m_pos_y += 1;
                if(m_pos_y > height())
                {
                    m_pos_y = 0;
                }
                ethereality->move(m_pos_x, m_pos_y + qrand() % max);
            }
            else
            {
                ethereality->start();
            }
        }
    }
    else
    {
        Florid::stop();
        foreach(Ethereality *ethereality, m_etherealitys)
        {
            ethereality->stop();
        }
    }
}

void FloridEthereality::hideEvent(QHideEvent *)
{
    m_timer->stop();
    foreach(Ethereality *ethereality, m_etherealitys)
    {
        ethereality->stop();
    }
}

void FloridEthereality::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
        foreach(Ethereality *ethereality, m_etherealitys)
        {
            ethereality->start();
        }
    }
}

void FloridEthereality::resizeEvent(QResizeEvent *)
{
    const int perWidth = width() / 50;
    for(int i = 0; i < m_etherealitys.size(); i++)
    {
        Ethereality *ethereality = m_etherealitys[i];
        ethereality->resize(perWidth, height());
        ethereality->move(i * perWidth, 0);
    }
}

void FloridEthereality::process()
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
        m_intern_vis_data[i] = int(m_left_buffer[pos >> 8] * m_rows * 1.0);
        m_intern_vis_data[i] = qBound(-m_rows / 2, m_intern_vis_data[i], m_rows / 2);
        m_intern_vis_data[m_cols * 2 - i - 1] = m_intern_vis_data[i];
    }
}
