#include "floridautism.h"

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
    m_enabled = true;
    m_offset = pos;
    m_crValue = LABEL_RADIUS;
    m_circleTimer->start();
}

void AutismLabel::setColor(const QColor &color)
{
    m_color = color;
}

void AutismLabel::updateRender()
{
    m_crValue += 2;
    if(m_crValue >= 2 * LABEL_RADIUS)
    {
        m_crValue = LABEL_RADIUS;
        m_enabled = false;
        m_circleTimer->stop();
    }

    update();
}

void AutismLabel::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    if(m_enabled)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setOpacity((2 * LABEL_RADIUS - m_crValue) * 0.8 / LABEL_RADIUS);
        painter.setPen(QPen(m_color, 2));
        painter.drawEllipse(m_offset, m_crValue, m_crValue);
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

    m_gradientLabel = false;
    m_timer->setInterval(700);
}

void FloridAutism::resizeEvent(QResizeEvent *)
{
    for(AutismLabel *label : qAsConst(m_labels))
    {
        label->setGeometry(0, 0, width(), height());
    }
    m_roundLabel->raise();
}

void FloridAutism::processData(float *, float *)
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
