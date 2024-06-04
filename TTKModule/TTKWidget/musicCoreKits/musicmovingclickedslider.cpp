#include "musicmovingclickedslider.h"

#include <QMouseEvent>

MusicMovingClickedSlider::MusicMovingClickedSlider(QWidget *parent)
    : MusicMovingClickedSlider(Qt::Horizontal, parent)
{

}

MusicMovingClickedSlider::MusicMovingClickedSlider(Qt::Orientation orientation, QWidget *parent)
    : TTKClickedSlider(orientation, parent),
      m_isMoving(false)
{
    setMouseTracking(true);
}

void MusicMovingClickedSlider::setValue(int value)
{
    if(!m_isMoving)
    {
        TTKClickedSlider::setValue(value);
    }
}

void MusicMovingClickedSlider::mousePressEvent(QMouseEvent *event)
{
    TTKClickedSlider::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_isMoving = false;
        Q_EMIT sliderMoved(m_value);
    }
}

void MusicMovingClickedSlider::mouseMoveEvent(QMouseEvent *event)
{
    TTKClickedSlider::mouseMoveEvent(event);
    if(m_mousePressed)
    {
        m_isMoving = true;
        Q_EMIT sliderMoved(m_value);
    }
}

void MusicMovingClickedSlider::mouseReleaseEvent(QMouseEvent *event)
{
    TTKClickedSlider::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        Q_EMIT sliderReleasedAt(m_value);
        m_isMoving = false;
    }
}
