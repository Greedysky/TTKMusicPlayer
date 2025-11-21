#include "ttkclickedslider.h"

#include <QMouseEvent>

TTKClickedSlider::TTKClickedSlider(QWidget *parent)
    : TTKClickedSlider(Qt::Horizontal, parent)
{

}

TTKClickedSlider::TTKClickedSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent),
      m_value(0),
      m_mousePressed(false)
{
    setCursor(QCursor(Qt::PointingHandCursor));
}

bool TTKClickedSlider::event(QEvent *event)
{
    if(m_mousePressed)
    {
        setValue(m_value);
    }
    return QSlider::event(event);
}

void TTKClickedSlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_mousePressed = true;

        double pos;
        if(orientation() == Qt::Horizontal)
        {
            pos = QtMouseX(event) * 1.0 / width();
            m_value = pos * (maximum() - minimum()) + minimum();
        }
        else
        {
            pos = QtMouseY(event) * 1.0 / height();
            m_value = maximum() - pos * (maximum() - minimum());
        }
    }
}

void TTKClickedSlider::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);
    if(m_mousePressed)
    {
        if(orientation() == Qt::Horizontal)
        {
            const int x = QtMouseX(event);
            if(x >= 0 && x <= width())
            {
                const double pos = QtMouseX(event) * 1.0 / width();
                m_value = pos * (maximum() - minimum()) + minimum();
                setValue(m_value);
            }
            else if(x < 0)
            {
                m_value = minimum();
            }
            else
            {
                m_value = maximum();
            }
        }
        else
        {
            const int y = QtMouseY(event);
            if(y >= 0 && y <= height())
            {
                const double pos = QtMouseY(event) * 1.0 / height();
                m_value = maximum() - pos * (maximum() - minimum());
                setValue(m_value);
            }
            else if(y < 0)
            {
                m_value = maximum();
            }
            else
            {
                m_value = minimum();
            }
        }

        Q_EMIT clicked();
    }
}

void TTKClickedSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    m_mousePressed = false;
}
