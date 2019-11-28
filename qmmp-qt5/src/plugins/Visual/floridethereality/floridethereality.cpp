#include <QPainter>
#include "fft.h"
#include "inlines.h"
#include "floridethereality.h"
#include "ethereality.h"

FloridEthereality::FloridEthereality (QWidget *parent) : Florid (parent)
{
    m_gradientOn = true;
    m_intern_vis_data = nullptr;
    m_running = false;
    m_rows = 0;
    m_cols = 0;

    setWindowTitle(tr("Florid Ethereality Widget"));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    m_timer->setInterval(QMMP_VISUAL_INTERVAL * 1.5);

    clear();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    m_etheral = new Ethereality(this);
    layout->addWidget(m_etheral);
    setLayout(layout);
}

FloridEthereality::~FloridEthereality()
{
    if(m_intern_vis_data)
    {
        delete[] m_intern_vis_data;
    }
    delete m_etheral;
}

void FloridEthereality::start()
{
    Florid::start();
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
        m_etheral->start();
    }
}

void FloridEthereality::stop()
{
    Florid::stop();
    m_running = false;
    m_timer->stop();
    m_etheral->stop();
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
        m_etheral->process(max * 3 + 200);
    }
    else
    {
        Florid::stop();
        m_etheral->stop();
    }
}

void FloridEthereality::hideEvent(QHideEvent *)
{
    m_timer->stop();
    m_etheral->stop();
}

void FloridEthereality::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
        m_etheral->start();
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

    const int step = (QMMP_VISUAL_NODE_SIZE << 8)/m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols * 2; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(m_left_buffer[pos >> 8] * m_rows* 1.0);
        m_intern_vis_data[i] = qBound(-m_rows/2, m_intern_vis_data[i], m_rows/2);
        m_intern_vis_data[m_cols * 2 - i - 1] = m_intern_vis_data[i];
    }
}
