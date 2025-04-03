#include "musicsmoothmovingwidget.h"
#include "musicwidgetheaders.h"

#include <QTimer>
#include <QWheelEvent>
#include <QPropertyAnimation>

MusicSmoothMovingInterface::MusicSmoothMovingInterface(QObject *parent)
    : QObject(parent),
      m_initialized(true),
      m_previousValue(0),
      m_deltaValue(0),
      m_scrollBar(nullptr),
      m_moveAnimation(nullptr)
{
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(100 * TTK_DN_MS);
    connect(m_animationTimer, SIGNAL(timeout()), SLOT(timeToAnimation()));
}

MusicSmoothMovingInterface::~MusicSmoothMovingInterface()
{
    m_animationTimer->stop();
    delete m_animationTimer;
    delete m_moveAnimation;
}

void MusicSmoothMovingInterface::setScrollBar(QScrollBar *bar)
{
    m_scrollBar = bar;
    delete m_moveAnimation;

    m_moveAnimation = new QPropertyAnimation(m_scrollBar, "value", this);
    m_moveAnimation->setDuration(TTK_DN_S2MS);
    connect(m_scrollBar, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
}

void MusicSmoothMovingInterface::updateValue(int value)
{
    if(!m_moveAnimation)
    {
        return;
    }

    m_animationTimer->stop();
    m_moveAnimation->stop();

    if(m_initialized)
    {
        m_deltaValue = 0;
        m_previousValue = m_scrollBar->value();
        m_initialized = false;
    }

    m_deltaValue += value;
    m_animationTimer->start();
}

void MusicSmoothMovingInterface::timeToAnimation()
{
    if(!m_moveAnimation)
    {
        return;
    }

    m_initialized = true;
    m_animationTimer->stop();

    m_deltaValue = (m_deltaValue / 480.0) * (m_deltaValue < 0 ? m_deltaValue + 120 : -m_deltaValue + 120);

    m_moveAnimation->setStartValue(m_previousValue);
    m_moveAnimation->setEndValue(m_scrollBar->value() + m_deltaValue);
    m_moveAnimation->start();
}

void MusicSmoothMovingInterface::valueChanged(int value)
{
    m_previousValue = value;
}



MusicSmoothMovingTableWidget::MusicSmoothMovingTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);
}

void MusicSmoothMovingTableWidget::setMovedScrollBar(QScrollBar *bar)
{
    m_interface.setScrollBar(bar == nullptr ? verticalScrollBar() : bar);
}

void MusicSmoothMovingTableWidget::wheelEvent(QWheelEvent *event)
{
    MusicAbstractTableWidget::wheelEvent(event);
    m_interface.updateValue(QtWheelDelta(event));
}



MusicSmoothMovingScrollArea::MusicSmoothMovingScrollArea(QWidget *parent)
    : QScrollArea(parent)
{
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);
}

void MusicSmoothMovingScrollArea::setMovedScrollBar(QScrollBar *bar)
{
    m_interface.setScrollBar(bar == nullptr ? verticalScrollBar() : bar);
}

void MusicSmoothMovingScrollArea::wheelEvent(QWheelEvent *event)
{
    QScrollArea::wheelEvent(event);
    m_interface.updateValue(QtWheelDelta(event));
}
