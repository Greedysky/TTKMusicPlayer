#include <QPainter>
#include "fft.h"
#include "inlines.h"
#include "floridautism.h"

CircleClickLabel::CircleClickLabel(QWidget *parent)
    : QWidget(parent)
{
    m_circleOn = false;
    m_crValue = DISTANCE;

    m_circleTimer = new QTimer(this);
    m_circleTimer->setInterval(QMMP_VISUAL_INTERVAL * 1.5);
    connect(m_circleTimer, SIGNAL(timeout()), SLOT(updateRender()));
}

CircleClickLabel::~CircleClickLabel()
{
    delete m_circleTimer;
}

void CircleClickLabel::start(const QPoint &pos)
{
    m_circleOn = true;
    m_pos = pos;
    m_crValue = DISTANCE;
    m_circleTimer->start();
}

void CircleClickLabel::setColor(const QColor &color)
{
    m_color = color;
}

void CircleClickLabel::updateRender()
{
    m_crValue += 2;
    if(m_crValue >= 2 * DISTANCE)
    {
        m_crValue = DISTANCE;
        m_circleOn = false;
        m_circleTimer->stop();
    }

    update();
}

void CircleClickLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if(m_circleOn)
    {
        QPainter painter(this);
        painter.setOpacity((2 * DISTANCE - m_crValue) * 0.8 / DISTANCE);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(m_color, 2));
        painter.drawEllipse(m_pos, m_crValue, m_crValue);
    }
}



FloridAutism::FloridAutism (QWidget *parent) : Florid (parent)
{
    m_index = 0;
    m_running = false;

    setWindowTitle(tr("Florid Autism Widget"));

    for(int i=0; i<4; ++i)
    {
        CircleClickLabel *label = new CircleClickLabel(this);
        label->setGeometry(0, 0, width(), height());
        m_labels << label;
    }

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_timer->setInterval(700);
}

FloridAutism::~FloridAutism()
{

}

void FloridAutism::start()
{
    Florid::start();
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
    }
}

void FloridAutism::stop()
{
    Florid::stop();
    m_running = false;
    m_timer->stop();
}

void FloridAutism::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        Florid::start();
        m_labels[m_index]->setColor(m_averageColor);
        m_labels[m_index]->start(rect().center());
        ++m_index;
        if(m_index >= m_labels.count())
        {
            m_index = 0;
        }
    }
    else
    {
        Florid::stop();
    }
}

void FloridAutism::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void FloridAutism::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void FloridAutism::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);
}
