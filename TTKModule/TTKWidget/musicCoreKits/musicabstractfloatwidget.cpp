#include "musicabstractfloatwidget.h"

MusicAbstractFloatWidget::MusicAbstractFloatWidget(QWidget *parent)
    : QLabel(parent),
      m_blockAnimation(false)
{
    m_animation = new QPropertyAnimation(this, "geometry", this);
    m_animation->setDuration(TTK_DN_S2MS / 2);
}

MusicAbstractFloatWidget::~MusicAbstractFloatWidget()
{
    delete m_animation;
}

void MusicAbstractFloatWidget::animationEnter()
{
    m_animation->setStartValue(m_rectLeave);
    m_animation->setEndValue(m_rectEnter);
    m_animation->start();
}

void MusicAbstractFloatWidget::animationLeave()
{
    m_animation->setStartValue(m_rectEnter);
    m_animation->setEndValue(m_rectLeave);
    m_animation->start();
}

void MusicAbstractFloatWidget::enterEvent(QtEnterEvent *event)
{
    QLabel::enterEvent(event);
    if(!m_blockAnimation)
    {
        animationEnter();
    }
}

void MusicAbstractFloatWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    if(!m_blockAnimation)
    {
        animationLeave();
    }
}
