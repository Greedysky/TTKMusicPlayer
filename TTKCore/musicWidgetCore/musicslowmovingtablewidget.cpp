#include "musicslowmovingtablewidget.h"

#include <QTimer>
#include <QScrollBar>
#include <QWheelEvent>
#include <QPropertyAnimation>

MusicSlowMovingTableWidget::MusicSlowMovingTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    m_deltaValue = 0;
    m_priviousValue = 0;
    m_isFirstInit = true;
    m_slowAnimation = nullptr;
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(100*MT_MS);
    connect(m_animationTimer, SIGNAL(timeout()), SLOT(timeToAnimation()));
}

MusicSlowMovingTableWidget::~MusicSlowMovingTableWidget()
{
    m_animationTimer->stop();
    delete m_animationTimer;
    delete m_slowAnimation;
}

QString MusicSlowMovingTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSlowMovingTableWidget::setMovedScrollBar(QScrollBar *bar)
{
    m_scrollBar = bar;
    delete m_slowAnimation;
    m_slowAnimation = new QPropertyAnimation(m_scrollBar, "value", this);
    m_slowAnimation->setDuration(MT_S2MS);
    connect(m_scrollBar, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
}

void MusicSlowMovingTableWidget::timeToAnimation()
{
    m_isFirstInit = true;
    m_animationTimer->stop();

    float delta = (rowCount() > 0) ? (height()*1.0/rowCount()) : 0;
    m_deltaValue = (m_deltaValue/480.0)*(m_deltaValue < 0 ? m_deltaValue + 120 : -m_deltaValue + 120);

    m_slowAnimation->setStartValue(m_priviousValue);
    m_slowAnimation->setEndValue(m_scrollBar->value() + m_deltaValue*delta/30);
    m_slowAnimation->start();
}

void MusicSlowMovingTableWidget::valueChanged(int value)
{
    m_priviousValue = value;
}

void MusicSlowMovingTableWidget::wheelEvent(QWheelEvent *event)
{
    MusicAbstractTableWidget::wheelEvent(event);

    m_animationTimer->stop();
    m_slowAnimation->stop();
    if(m_isFirstInit)
    {
        m_deltaValue = 0;
        m_priviousValue = m_scrollBar->value();
        m_isFirstInit = false;
    }
    m_deltaValue += event->delta();
    m_animationTimer->start();
}
