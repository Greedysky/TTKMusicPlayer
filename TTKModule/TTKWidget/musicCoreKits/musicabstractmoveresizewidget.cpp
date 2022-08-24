#include "musicabstractmoveresizewidget.h"

#include <QMouseEvent>
#include <QApplication>

#define DISTANCE  5

MusicAbstractMoveResizeWidget::MusicAbstractMoveResizeWidget(QWidget *parent)
    : QWidget(parent)
{
    m_struct.m_mouseLeftPress = false;
    m_struct.m_isPressBorder = false;
    m_direction = Direction::No;

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);
}

bool MusicAbstractMoveResizeWidget::eventFilter(QObject *object, QEvent *event)
{
    QWidget::eventFilter(object, event);
    if(QEvent::MouseMove == event->type())
    {
        QMouseEvent *mouseEvent = TTKStatic_cast(QMouseEvent*, event);
        QApplication::sendEvent(this, mouseEvent);
    }
    return false;
}

void MusicAbstractMoveResizeWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if(m_struct.m_isPressBorder || m_direction == Direction::No)
    {
        return;
    }

    const QPoint &point = mapFromGlobal(QCursor::pos());
    if(point.y() > DISTANCE && point.y() < height() - DISTANCE && point.x() > DISTANCE && point.x() < width() - DISTANCE)
    {
        setCursor(Qt::ArrowCursor);
        m_direction = Direction::No;
    }
}

void MusicAbstractMoveResizeWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);

    if(event->buttons() == Qt::LeftButton)
    {
        isMaximized() ? showNormal() : showMaximized();
    }
}

void MusicAbstractMoveResizeWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    m_struct.m_pressedSize = size();
    m_struct.m_isPressBorder = false;
    setFocus();

    if(event->button() == Qt::LeftButton)
    {
        m_struct.m_windowPos = pos();
        if(QRect(DISTANCE + 1, DISTANCE + 1, width() - (DISTANCE + 1) * 2, height() - (DISTANCE + 1) * 2).contains(event->pos()))
        {
            m_struct.m_mousePos = QtMouseEventGlobalPos(event);
            m_struct.m_mouseLeftPress = true;
        }
        else
        {
            m_struct.m_isPressBorder = true;
        }
    }
}

void MusicAbstractMoveResizeWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
#ifndef Q_OS_UNIX
    !m_struct.m_isPressBorder ? sizeDirection() : moveDirection();
#endif

    if(m_struct.m_mouseLeftPress)
    {
        move(m_struct.m_windowPos + QtMouseEventGlobalPos(event) - m_struct.m_mousePos);
    }
}

void MusicAbstractMoveResizeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_struct.m_isPressBorder = false;
    m_struct.m_mouseLeftPress = false;
    setCursor(QCursor(Qt::ArrowCursor));
    m_direction = Direction::No;
}

void MusicAbstractMoveResizeWidget::sizeDirection()
{
    const QPoint &point = mapFromGlobal(QCursor::pos());
    if(point.x() > width() - DISTANCE && point.y() < height() - DISTANCE && point.y() > DISTANCE)
    {
        setCursor(Qt::SizeHorCursor);
        m_direction = Direction::Right;
    }
    else if(point.x() < DISTANCE && point.y() < height() - DISTANCE && point.y() > DISTANCE)
    {
        setCursor(Qt::SizeHorCursor);
        m_direction = Direction::Left;
    }
    else if(point.y() > height() - DISTANCE && point.x() > DISTANCE && point.x() < width() - DISTANCE)
    {
        setCursor(Qt::SizeVerCursor);
        m_direction = Direction::Bottom;
    }
    else if(point.y() < DISTANCE && point.x() > DISTANCE && point.x() < width() - DISTANCE)
    {
        setCursor(Qt::SizeVerCursor);
        m_direction = Direction::Top;
    }
    else if(point.y() < DISTANCE && point.x() > width() - DISTANCE)
    {
        setCursor(Qt::SizeBDiagCursor);
        m_direction = Direction::RightTop;
    }
    else if(point.y() < DISTANCE && point.x() < DISTANCE)
    {
        setCursor(Qt::SizeFDiagCursor);
        m_direction = Direction::LeftTop;
    }
    else if(point.x() > DISTANCE && point.y() > height() - DISTANCE)
    {
        setCursor(Qt::SizeFDiagCursor);
        m_direction = Direction::RightBottom;
    }
    else if(point.x() < DISTANCE && point.y() > height() - DISTANCE)
    {
        setCursor(Qt::SizeBDiagCursor);
        m_direction = Direction::LeftBottom;
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        m_direction = Direction::No;
    }
}

void MusicAbstractMoveResizeWidget::moveDirection()
{
    switch(m_direction)
    {
        case Direction::Right:
        {
            const int wValue = QCursor::pos().x() - x();
            if(minimumWidth() <= wValue && wValue <= maximumWidth())
            {
                setGeometry(x(), y(), wValue, height());
            }
            break;
        }
        case Direction::Left:
        {
            const int wValue = x() + width() - QCursor::pos().x();
            if(minimumWidth() <= wValue && wValue <= maximumWidth())
            {
                setGeometry(QCursor::pos().x(), y(), wValue, height());
            }
            break;
        }
        case Direction::Bottom:
        {
            const int hValue = QCursor::pos().y() - y();
            if(minimumHeight() <= hValue && hValue <= maximumHeight())
            {
                setGeometry(x(), y(), width(), hValue);
            }
            break;
        }
        case Direction::Top:
        {
            const int hValue = y() - QCursor::pos().y() + height();
            if(minimumHeight() <= hValue && hValue <= maximumHeight())
            {
                setGeometry(x(), QCursor::pos().y(), width(), hValue);
            }
            break;
        }
        case Direction::RightTop:
        {
            int hValue = y() + height() - QCursor::pos().y();
            const int wValue = QCursor::pos().x() - x();
            int yValue = QCursor::pos().y();
            if(hValue >= maximumHeight())
            {
                yValue = m_struct.m_windowPos.y() + m_struct.m_pressedSize.height() - height();
                hValue = maximumHeight();
            }

            if(hValue <= minimumHeight())
            {
                yValue = m_struct.m_windowPos.y() + m_struct.m_pressedSize.height() - height();
                hValue = minimumHeight();
            }
            setGeometry(m_struct.m_windowPos.x(), yValue, wValue, hValue);
            break;
        }
        case Direction::LeftTop:
        {
            int yValue = QCursor::pos().y();
            int xValue = QCursor::pos().x();

            int wValue = pos().x() + width()- xValue;
            int hValue = pos().y() + height() - yValue;

            const int twValue = m_struct.m_windowPos.x() + m_struct.m_pressedSize.width();
            const int thValue = m_struct.m_windowPos.y() + m_struct.m_pressedSize.height();

            if(twValue - xValue >= maximumWidth())
            {
                xValue = twValue - maximumWidth();
                wValue = maximumWidth();
            }

            if(twValue - xValue <= minimumWidth())
            {
                xValue = twValue - minimumWidth();
                wValue = minimumWidth();
            }

            if(thValue - yValue >= maximumHeight())
            {
                yValue = thValue - maximumHeight();
                hValue = maximumHeight();
            }

            if(thValue - yValue <= minimumHeight())
            {
                yValue = thValue - minimumHeight();
                hValue = minimumHeight();
            }
            setGeometry(xValue, yValue, wValue, hValue);
            break;
        }
        case Direction::RightBottom:
        {
            const int wValue = QCursor::pos().x() - x();
            const int hValue = QCursor::pos().y() - y();
            setGeometry(m_struct.m_windowPos.x(), m_struct.m_windowPos.y(), wValue, hValue);
            break;
        }
        case Direction::LeftBottom:
        {
            int wValue = x() + width() - QCursor::pos().x();
            const int hValue = QCursor::pos().y() - m_struct.m_windowPos.y();
            int xValue = QCursor::pos().x();
            const int twValue = m_struct.m_windowPos.x() + m_struct.m_pressedSize.width();

            if(twValue - xValue >= maximumWidth())
            {
                xValue = twValue - maximumWidth();
                wValue = maximumWidth();
            }

            if(twValue - xValue <= minimumWidth())
            {
                xValue = twValue - minimumWidth();
                wValue = minimumWidth();
            }
            setGeometry(xValue, m_struct.m_windowPos.y(), wValue, hValue);
            break;
        }
        default: break;
    }
}
