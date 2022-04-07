#include "musicabstractmoveresizewidget.h"

#include <QMouseEvent>
#include <QApplication>

#define DISTANCE  5

MusicAbstractMoveResizeWidget::MusicAbstractMoveResizeWidget(QWidget *parent)
    : QWidget(parent)
{
    m_struct.m_mouseLeftPress = false;
    m_struct.m_isPressBorder = false;
    m_direction = DirectionNo;

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
    if(m_struct.m_isPressBorder || m_direction == DirectionNo)
    {
        return;
    }

    const QPoint &point = mapFromGlobal(QCursor::pos());
    if(point.y() > DISTANCE && point.y() < height() - DISTANCE && point.x() > DISTANCE && point.x() < width() - DISTANCE)
    {
        setCursor(Qt::ArrowCursor);
        m_direction = DirectionNo;
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
#if TTK_QT_VERSION_CHECK(6,0,0)
            m_struct.m_mousePos = event->globalPosition().toPoint();
#else
            m_struct.m_mousePos = event->globalPos();
#endif
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
#if TTK_QT_VERSION_CHECK(6,0,0)
        move(m_struct.m_windowPos + (event->globalPosition().toPoint() - m_struct.m_mousePos));
#else
        move(m_struct.m_windowPos + (event->globalPos() - m_struct.m_mousePos));
#endif
    }
}

void MusicAbstractMoveResizeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_struct.m_isPressBorder = false;
    m_struct.m_mouseLeftPress = false;
    setCursor(QCursor(Qt::ArrowCursor));
    m_direction = DirectionNo;
}

void MusicAbstractMoveResizeWidget::sizeDirection()
{
    const QPoint &point = mapFromGlobal(QCursor::pos());
    if(point.x() > width() - DISTANCE && point.y() < height() - DISTANCE && point.y() > DISTANCE)
    {
        setCursor(Qt::SizeHorCursor);
        m_direction = DirectionRight;
    }
    else if(point.x() < DISTANCE && point.y() < height() - DISTANCE && point.y() > DISTANCE)
    {
        setCursor(Qt::SizeHorCursor);
        m_direction = DirectionLeft;
    }
    else if(point.y() > height() - DISTANCE && point.x() > DISTANCE && point.x() < width() - DISTANCE)
    {
        setCursor(Qt::SizeVerCursor);
        m_direction = DirectionBottom;
    }
    else if(point.y() < DISTANCE && point.x() > DISTANCE && point.x() < width() - DISTANCE)
    {
        setCursor(Qt::SizeVerCursor);
        m_direction = DirectionTop;
    }
    else if(point.y() < DISTANCE && point.x() > width() - DISTANCE)
    {
        setCursor(Qt::SizeBDiagCursor);
        m_direction = DirectionRightTop;
    }
    else if(point.y() < DISTANCE && point.x() < DISTANCE)
    {
        setCursor(Qt::SizeFDiagCursor);
        m_direction = DirectionLeftTop;
    }
    else if(point.x() > DISTANCE && point.y() > height() - DISTANCE)
    {
        setCursor(Qt::SizeFDiagCursor);
        m_direction = DirectionRightBottom;
    }
    else if(point.x() < DISTANCE && point.y() > height() - DISTANCE)
    {
        setCursor(Qt::SizeBDiagCursor);
        m_direction = DirectionLeftBottom;
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        m_direction = DirectionNo;
    }
}

void MusicAbstractMoveResizeWidget::moveDirection()
{
    switch(m_direction)
    {
        case DirectionRight:
        {
            const int wValue = QCursor::pos().x() - x();
            if(minimumWidth() <= wValue && wValue <= maximumWidth())
            {
                setGeometry(x(), y(), wValue, height());
            }
            break;
        }
        case DirectionLeft:
        {
            const int wValue = x() + width() - QCursor::pos().x();
            if(minimumWidth() <= wValue && wValue <= maximumWidth())
            {
                setGeometry(QCursor::pos().x(), y(), wValue, height());
            }
            break;
        }
        case DirectionBottom:
        {
            const int hValue = QCursor::pos().y() - y();
            if(minimumHeight() <= hValue && hValue <= maximumHeight())
            {
                setGeometry(x(), y(), width(), hValue);
            }
            break;
        }
        case DirectionTop:
        {
            const int hValue = y() - QCursor::pos().y() + height();
            if(minimumHeight() <= hValue && hValue <= maximumHeight())
            {
                setGeometry(x(), QCursor::pos().y(), width(), hValue);
            }
            break;
        }
        case DirectionRightTop:
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
        case DirectionLeftTop:
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
        case DirectionRightBottom:
        {
            const int wValue = QCursor::pos().x() - x();
            const int hValue = QCursor::pos().y() - y();
            setGeometry(m_struct.m_windowPos.x(), m_struct.m_windowPos.y(), wValue, hValue);
            break;
        }
        case DirectionLeftBottom:
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
