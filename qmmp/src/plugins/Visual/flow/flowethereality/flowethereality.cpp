#include <QPainter>
#include "fft.h"
#include "inlines.h"
#include "flowethereality.h"
#include "ethereality.h"

FlowEthereality::FlowEthereality(QWidget *parent)
    : Florid(parent)
{
    m_useImage = false;
    m_pos_x = 0;
    m_pos_y = 0;

    qsrand(QDateTime::currentMSecsSinceEpoch());
    setWindowTitle(tr("Flow Ethereality Widget"));

    for(int i = 0; i < 50; i++)
    {
        Ethereality *ethereality = new Ethereality(this);
        ethereality->resize(20, 200);
        ethereality->move(i * 20, 0);
        m_etherealitys << ethereality;
    }
}

FlowEthereality::~FlowEthereality()
{
    qDeleteAll(m_etherealitys);
}

void FlowEthereality::start()
{
    Florid::start();
    if(isVisible())
    {
        foreach(Ethereality *ethereality, m_etherealitys)
        {
            ethereality->start();
        }
    }
}

void FlowEthereality::stop()
{
    Florid::stop();
    foreach(Ethereality *ethereality, m_etherealitys)
    {
        ethereality->stop();
    }
}

void FlowEthereality::hideEvent(QHideEvent *e)
{
    Florid::hideEvent(e);
    foreach(Ethereality *ethereality, m_etherealitys)
    {
        ethereality->stop();
    }
}

void FlowEthereality::showEvent(QShowEvent *e)
{
    Florid::showEvent(e);
    if(m_running)
    {
        foreach(Ethereality *ethereality, m_etherealitys)
        {
            ethereality->start();
        }
    }
}

void FlowEthereality::resizeEvent(QResizeEvent *)
{
    const int perWidth = width() / 50;
    for(int i = 0; i < m_etherealitys.size(); i++)
    {
        Ethereality *ethereality = m_etherealitys[i];
        ethereality->resize(perWidth, height());
        ethereality->move(i * perWidth, 0);
    }
}

void FlowEthereality::process(float *left, float *)
{
    static fft_state *state = nullptr;
    if(!state)
    {
        state = fft_init();
    }

    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        m_intern_vis_data = new int[m_cols * 2]{0};
    }

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols * 2; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(left[pos >> 8] * m_rows / 2);
        m_intern_vis_data[i] = qBound(-m_rows / 2, m_intern_vis_data[i], m_rows / 2);
        m_intern_vis_data[m_cols * 2 - i - 1] = m_intern_vis_data[i];
    }
    //
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
