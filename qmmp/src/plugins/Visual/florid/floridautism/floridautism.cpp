#include "floridautism.h"
#include "inlines.h"
#include "fft.h"

#include <QPainter>

#define ANIMATION_SIZE  4

AutismLabel::AutismLabel(QWidget *parent)
    : QWidget(parent)
{
    m_circleTimer = new QTimer(this);
    m_circleTimer->setInterval(QMMP_VISUAL_INTERVAL * 1.5);
    connect(m_circleTimer, SIGNAL(timeout()), SLOT(updateRender()));
}

AutismLabel::~AutismLabel()
{
    delete m_circleTimer;
}

void AutismLabel::start(const QPoint &pos)
{
    m_circleOn = true;
    m_pos = pos;
    m_crValue = DISTANCE;
    m_circleTimer->start();
}

void AutismLabel::setColor(const QColor &color)
{
    m_color = color;
}

void AutismLabel::updateRender()
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

void AutismLabel::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    if(m_circleOn)
    {
        QPainter painter(this);
        painter.setOpacity((2 * DISTANCE - m_crValue) * 0.8 / DISTANCE);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(m_color, 2));
        painter.drawEllipse(m_pos, m_crValue, m_crValue);
    }
}



FloridAutism::FloridAutism(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Autism Widget"));

    for(int i = 0; i < ANIMATION_SIZE; ++i)
    {
        AutismLabel *label = new AutismLabel(this);
        m_labels << label;
    }

    m_timer->setInterval(700);
}

FloridAutism::~FloridAutism()
{

}

void FloridAutism::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);
}

void FloridAutism::resizeEvent(QResizeEvent *e)
{
    Florid::resizeEvent(e);
    for(AutismLabel *label : qAsConst(m_labels))
    {
        label->setGeometry(0, 0, width(), height());
    }
    m_roundLabel->raise();
}

void FloridAutism::process(float *, float *)
{
    AutismLabel *label = m_labels[m_index];
    label->setColor(m_averageColor);
    label->start(rect().center());

    ++m_index;
    if(m_index >= m_labels.count())
    {
        m_index = 0;
    }
}
