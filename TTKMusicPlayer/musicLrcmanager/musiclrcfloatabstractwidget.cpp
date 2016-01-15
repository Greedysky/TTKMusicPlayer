#include "musiclrcfloatabstractwidget.h"

MusicLrcFloatAbstractWidget::MusicLrcFloatAbstractWidget(QWidget *parent)
    : QLabel(parent)
{
    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setDuration(500);
    m_blockAnimation = false;
}

MusicLrcFloatAbstractWidget::~MusicLrcFloatAbstractWidget()
{
    delete m_animation;
}

void MusicLrcFloatAbstractWidget::animationIn()
{
    m_animation->setStartValue(m_rectOut);
    m_animation->setEndValue(m_rectIn);
    m_animation->start();
}

void MusicLrcFloatAbstractWidget::animationOut()
{
    m_animation->setStartValue(m_rectIn);
    m_animation->setEndValue(m_rectOut);
    m_animation->start();
}

void MusicLrcFloatAbstractWidget::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    if(!m_blockAnimation)
    {
        animationIn();
    }
}

void MusicLrcFloatAbstractWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    if(!m_blockAnimation)
    {
        animationOut();
    }
}
