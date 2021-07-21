#include "musicabstractfloatwidget.h"

MusicAbstractFloatWidget::MusicAbstractFloatWidget(QWidget *parent)
    : QLabel(parent)
{
    m_animation = new QPropertyAnimation(this, "geometry", this);
    m_animation->setDuration(500);
    m_blockAnimation = false;
}

MusicAbstractFloatWidget::~MusicAbstractFloatWidget()
{
    delete m_animation;
}

void MusicAbstractFloatWidget::animationIn()
{
    m_animation->setStartValue(m_rectOut);
    m_animation->setEndValue(m_rectIn);
    m_animation->start();
}

void MusicAbstractFloatWidget::animationOut()
{
    m_animation->setStartValue(m_rectIn);
    m_animation->setEndValue(m_rectOut);
    m_animation->start();
}

#if TTK_QT_VERSION_CHECK(6,0,0)
void MusicAbstractFloatWidget::enterEvent(QEnterEvent *event)
#else
void MusicAbstractFloatWidget::enterEvent(QEvent *event)
#endif
{
    QLabel::enterEvent(event);
    if(!m_blockAnimation)
    {
        animationIn();
    }
}

void MusicAbstractFloatWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    if(!m_blockAnimation)
    {
        animationOut();
    }
}
