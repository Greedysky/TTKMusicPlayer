#include "flowethereality.h"
#include "ethereality.h"
#include "inlines.h"

#include <qmmp/qmmp.h>
#include <QPainter>
#include <QPaintEvent>

FlowEthereality::FlowEthereality(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow Ethereality Widget"));

    qsrand(QDateTime::currentMSecsSinceEpoch());

    for(int i = 0; i < 50; ++i)
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

void FlowEthereality::resizeEvent(QResizeEvent *)
{
    const int perWidth = width() / 50;
    for(int i = 0; i < m_etherealitys.count(); ++i)
    {
        Ethereality *ethereality = m_etherealitys[i];
        ethereality->resize(perWidth, height());
        ethereality->move(i * perWidth, 0);
    }
}

void FlowEthereality::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), Qt::black);
}

void FlowEthereality::process(float *left, float *)
{
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
    for(Ethereality *ethereality : qAsConst(m_etherealitys))
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

void FlowEthereality::processPatch(bool state)
{
    Visual::processPatch(state);

    if(state)
    {
        for(Ethereality *ethereality : qAsConst(m_etherealitys))
        {
            ethereality->start();
        }
    }
    else
    {
        for(Ethereality *ethereality : qAsConst(m_etherealitys))
        {
            ethereality->stop();
        }
    }
}
