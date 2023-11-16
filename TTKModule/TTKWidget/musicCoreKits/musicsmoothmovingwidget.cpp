#include "musicsmoothmovingwidget.h"
#include "musicwidgetheaders.h"

#include <QTimer>
#include <QWheelEvent>
#include <QPropertyAnimation>

MusicSmoothMovingTableWidget::MusicSmoothMovingTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent),
      m_initialized(true),
      m_previousValue(0),
      m_deltaValue(0),
      m_scrollBar(nullptr),
      m_slowAnimation(nullptr)
{
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(100);
    connect(m_animationTimer, SIGNAL(timeout()), SLOT(timeToAnimation()));
}

MusicSmoothMovingTableWidget::~MusicSmoothMovingTableWidget()
{
    m_animationTimer->stop();
    delete m_animationTimer;
    delete m_slowAnimation;
}

void MusicSmoothMovingTableWidget::setMovedScrollBar(QScrollBar *bar)
{
    m_scrollBar = (bar == nullptr) ? verticalScrollBar() : bar;
    delete m_slowAnimation;

    m_slowAnimation = new QPropertyAnimation(m_scrollBar, "value", this);
    m_slowAnimation->setDuration(TTK_DN_S2MS);
    connect(m_scrollBar, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
}

void MusicSmoothMovingTableWidget::timeToAnimation()
{
    if(!m_slowAnimation)
    {
        return;
    }

    m_initialized = true;
    m_animationTimer->stop();

    m_deltaValue = (m_deltaValue / 480.0) * (m_deltaValue < 0 ? m_deltaValue + 120 : -m_deltaValue + 120);

    m_slowAnimation->setStartValue(m_previousValue);
    m_slowAnimation->setEndValue(m_scrollBar->value() + m_deltaValue * TTK_ITEM_SIZE_M / 30);
    m_slowAnimation->start();
}

void MusicSmoothMovingTableWidget::valueChanged(int value)
{
    m_previousValue = value;
}

void MusicSmoothMovingTableWidget::wheelEvent(QWheelEvent *event)
{
    MusicAbstractTableWidget::wheelEvent(event);

    if(!m_slowAnimation)
    {
        return;
    }

    m_animationTimer->stop();
    m_slowAnimation->stop();

    if(m_initialized)
    {
        m_deltaValue = 0;
        m_previousValue = m_scrollBar->value();
        m_initialized = false;
    }

    m_deltaValue += QtWheelDelta(event);
    m_animationTimer->start();
}



MusicSmoothMovingScrollArea::MusicSmoothMovingScrollArea(QWidget *parent)
    : QScrollArea(parent),
      m_initialized(true),
      m_previousValue(0),
      m_deltaValue(0),
      m_scrollBar(nullptr),
      m_slowAnimation(nullptr)
{
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(100);

    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);
    setAlignment(Qt::AlignLeft);

    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    connect(m_animationTimer, SIGNAL(timeout()), SLOT(timeToAnimation()));
}

MusicSmoothMovingScrollArea::~MusicSmoothMovingScrollArea()
{
    m_animationTimer->stop();
    delete m_animationTimer;
    delete m_slowAnimation;
}

void MusicSmoothMovingScrollArea::setMovedScrollBar(QScrollBar *bar)
{
    m_scrollBar = (bar == nullptr) ? verticalScrollBar() : bar;
    delete m_slowAnimation;
    m_slowAnimation = new QPropertyAnimation(m_scrollBar, "value", this);
    m_slowAnimation->setDuration(TTK_DN_S2MS);
    connect(m_scrollBar, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
}

void MusicSmoothMovingScrollArea::timeToAnimation()
{
    if(!m_slowAnimation)
    {
        return;
    }

    m_initialized = true;
    m_animationTimer->stop();

    m_deltaValue = (m_deltaValue / 480.0) * (m_deltaValue < 0 ? m_deltaValue + 120 : -m_deltaValue + 120);

    m_slowAnimation->setStartValue(m_previousValue);
    m_slowAnimation->setEndValue(m_scrollBar->value() + m_deltaValue * TTK_ITEM_SIZE_M / 30);
    m_slowAnimation->start();
}

void MusicSmoothMovingScrollArea::valueChanged(int value)
{
    m_previousValue = value;
}

void MusicSmoothMovingScrollArea::wheelEvent(QWheelEvent *event)
{
    QScrollArea::wheelEvent(event);

    if(!m_slowAnimation)
    {
        return;
    }

    m_animationTimer->stop();
    m_slowAnimation->stop();

    if(m_initialized)
    {
        m_deltaValue = 0;
        m_previousValue = m_scrollBar->value();
        m_initialized = false;
    }

    m_deltaValue += QtWheelDelta(event);
    m_animationTimer->start();
}
