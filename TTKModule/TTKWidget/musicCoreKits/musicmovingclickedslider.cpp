#include "musicmovingclickedslider.h"

#include <QMouseEvent>

MusicMovingClickedSlider::MusicMovingClickedSlider(QWidget *parent)
    : MusicMovingClickedSlider(Qt::Horizontal, parent)
{

}

MusicMovingClickedSlider::MusicMovingClickedSlider(Qt::Orientation orientation, QWidget *parent)
    : MusicClickedSlider(orientation, parent)
{
    setMouseTracking(true);

    m_isMoving = false;
    m_orientation = orientation;
}

void MusicMovingClickedSlider::setValue(int value)
{
    if(!m_isMoving)
    {
        MusicClickedSlider::setValue(value);
    }
}

void MusicMovingClickedSlider::mousePressEvent(QMouseEvent *event)
{
    MusicClickedSlider::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_isMoving = false;
        Q_EMIT sliderMoved( m_value );
    }
}

void MusicMovingClickedSlider::mouseMoveEvent(QMouseEvent *event)
{
    MusicClickedSlider::mouseMoveEvent(event);
    if(m_mousePress)
    {
        m_isMoving = true;
        Q_EMIT sliderMoved( m_value );
    }
}

void MusicMovingClickedSlider::mouseReleaseEvent(QMouseEvent *event)
{
    MusicClickedSlider::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        Q_EMIT sliderReleasedAt( m_value );
        m_isMoving = false;
    }
}
