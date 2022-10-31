#include "ttkabstractmoveresizewidget.h"

#include <QApplication>

#define DISTANCE  5

#if !TTK_QT_VERSION_CHECK(5,0,0) && defined(Q_OS_UNIX)
#  define SET_GEOMETRY(x, y, w, h) move(x, y); resize(w, h);
#else
#  define SET_GEOMETRY(x, y, w, h) setGeometry(x, y, w, h);
#endif

TTKAbstractMoveResizeWidget::TTKAbstractMoveResizeWidget(QWidget *parent)
    : QWidget(parent),
      m_direction(Direction::No)
{
    m_struct.m_mouseLeftPress = false;
    m_struct.m_isPressBorder = false;

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);
}

bool TTKAbstractMoveResizeWidget::eventFilter(QObject *object, QEvent *event)
{
    QWidget::eventFilter(object, event);
    if(QEvent::MouseMove == event->type())
    {
        QMouseEvent *mouseEvent = TTKStatic_cast(QMouseEvent*, event);
        QApplication::sendEvent(this, mouseEvent);
    }
    return false;
}

void TTKAbstractMoveResizeWidget::paintEvent(QPaintEvent *event)
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

void TTKAbstractMoveResizeWidget::mousePressEvent(QMouseEvent *event)
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

void TTKAbstractMoveResizeWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    !m_struct.m_isPressBorder ? sizeDirection() : moveDirection();

    if(m_struct.m_mouseLeftPress)
    {
        move(m_struct.m_windowPos + QtMouseEventGlobalPos(event) - m_struct.m_mousePos);
    }
}

void TTKAbstractMoveResizeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_struct.m_isPressBorder = false;
    m_struct.m_mouseLeftPress = false;
    setCursor(QCursor(Qt::ArrowCursor));
    m_direction = Direction::No;
}

void TTKAbstractMoveResizeWidget::sizeDirection()
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

void TTKAbstractMoveResizeWidget::moveDirection()
{
    const QPoint &point = QCursor::pos();
    switch(m_direction)
    {
        case Direction::Right:
        {
            const int wValue = point.x() - x();
            if(minimumWidth() <= wValue && wValue <= maximumWidth())
            {
                SET_GEOMETRY(x(), y(), wValue, height());
            }
            break;
        }
        case Direction::Left:
        {
            const int wValue = x() + width() - point.x();
            if(minimumWidth() <= wValue && wValue <= maximumWidth())
            {
                SET_GEOMETRY(point.x(), y(), wValue, height());
            }
            break;
        }
        case Direction::Bottom:
        {
            const int hValue = point.y() - y();
            if(minimumHeight() <= hValue && hValue <= maximumHeight())
            {
                SET_GEOMETRY(x(), y(), width(), hValue);
            }
            break;
        }
        case Direction::Top:
        {
            const int hValue = y() - point.y() + height();
            if(minimumHeight() <= hValue && hValue <= maximumHeight())
            {
                SET_GEOMETRY(x(), point.y(), width(), hValue);
            }
            break;
        }
        case Direction::RightTop:
        {
            int hValue = y() + height() - point.y();
            const int wValue = point.x() - x();
            int yValue = point.y();

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

            SET_GEOMETRY(m_struct.m_windowPos.x(), yValue, wValue, hValue);
            break;
        }
        case Direction::LeftTop:
        {
            int yValue = point.y();
            int xValue = point.x();

            int wValue = pos().x() + width() - xValue;
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

            SET_GEOMETRY(xValue, yValue, wValue, hValue);
            break;
        }
        case Direction::RightBottom:
        {
            const int wValue = point.x() - x();
            const int hValue = point.y() - y();
            SET_GEOMETRY(m_struct.m_windowPos.x(), m_struct.m_windowPos.y(), wValue, hValue);
            break;
        }
        case Direction::LeftBottom:
        {
            int wValue = x() + width() - point.x();
            const int hValue = point.y() - m_struct.m_windowPos.y();
            int xValue = point.x();
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

            SET_GEOMETRY(xValue, m_struct.m_windowPos.y(), wValue, hValue);
            break;
        }
        default: break;
    }
}

QObjectList TTKAbstractMoveResizeWidget::foreachWidget(QObject *object)
{
    QObjectList result;
    for(QObject *obj : object->children())
    {
        if("QWidget" == QString(obj->metaObject()->className()))
        {
            result.append(obj);
        }
        result += foreachWidget(obj);
    }
    return result;
}
