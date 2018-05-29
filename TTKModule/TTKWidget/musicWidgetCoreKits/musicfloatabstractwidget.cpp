#include "musicfloatabstractwidget.h"

MusicFloatAbstractWidget::MusicFloatAbstractWidget(QWidget *parent)
    : QLabel(parent)
{
    m_animation = new QPropertyAnimation(this, "geometry", this);
    m_animation->setDuration(500);
    m_blockAnimation = false;
}

MusicFloatAbstractWidget::~MusicFloatAbstractWidget()
{
    delete m_animation;
}

void MusicFloatAbstractWidget::animationIn()
{
    m_animation->setStartValue(m_rectOut);
    m_animation->setEndValue(m_rectIn);
    m_animation->start();
}

void MusicFloatAbstractWidget::animationOut()
{
    m_animation->setStartValue(m_rectIn);
    m_animation->setEndValue(m_rectOut);
    m_animation->start();
}

void MusicFloatAbstractWidget::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    if(!m_blockAnimation)
    {
        animationIn();
    }
}

void MusicFloatAbstractWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    if(!m_blockAnimation)
    {
        animationOut();
    }
}
