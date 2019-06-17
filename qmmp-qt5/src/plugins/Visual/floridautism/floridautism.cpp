#include <QPainter>
#include "fft.h"
#include "inlines.h"
#include "floridautism.h"

CircleClickLabel::CircleClickLabel(QWidget *parent)
    : QWidget(parent)
{
    m_circleOn = false;
    m_crValue = 4;

    m_circleTimer = new QTimer(this);
    m_circleTimer->setInterval(QMMP_VISUAL_INTERVAL);
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
    m_crValue = 4;
    m_circleTimer->start();
}

void CircleClickLabel::updateRender()
{
    m_crValue += 2;
    if(m_crValue >= 40)
    {
        m_crValue = 4;
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
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(QColor(20, 203, 232, 50), 3));
        painter.drawEllipse(m_pos, m_crValue, m_crValue);
        painter.setOpacity((40 - m_crValue) * 1.0 / 40);
    }
}



FloridAutism::FloridAutism (QWidget *parent) : Florid (parent)
{
    m_running = false;

    setWindowTitle(tr("Florid Autism Widget"));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    m_timer->setInterval(QMMP_VISUAL_INTERVAL);

    clear();
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
    clear();
}

void FloridAutism::clear()
{
    update();
}

void FloridAutism::timeout()
{
    update();
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
    QPainter painter(this);
    draw(&painter);
}

void FloridAutism::draw(QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing);
    p->setPen(QPen(QColor(64, 229, 255), 3));
    p->translate(rect().center());
}
