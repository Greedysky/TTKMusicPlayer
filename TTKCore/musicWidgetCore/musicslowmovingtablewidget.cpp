#include "musicslowmovingtablewidget.h"

#include <QScrollBar>
#include <QWheelEvent>
#include <QPropertyAnimation>

MusicSlowMovingTableWidget::MusicSlowMovingTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    m_priviousValue = 0;
    m_slowAnimation = nullptr;
}

MusicSlowMovingTableWidget::~MusicSlowMovingTableWidget()
{
    delete m_slowAnimation;
}

QString MusicSlowMovingTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSlowMovingTableWidget::setMovedScrollBar(QScrollBar *bar)
{
    m_scrollBar = bar;
    m_slowAnimation = new QPropertyAnimation(m_scrollBar, "value");
    m_slowAnimation->setDuration(1000);
    connect(m_scrollBar, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
}

void MusicSlowMovingTableWidget::valueChanged(int value)
{
    m_priviousValue = value;
}

void MusicSlowMovingTableWidget::wheelEvent(QWheelEvent *event)
{
    MusicAbstractTableWidget::wheelEvent(event);

    m_slowAnimation->stop();
    m_slowAnimation->setStartValue(m_priviousValue);
    m_priviousValue = m_scrollBar->value() - event->delta()*5;
    m_slowAnimation->setEndValue(m_priviousValue);
    m_slowAnimation->start();
}
