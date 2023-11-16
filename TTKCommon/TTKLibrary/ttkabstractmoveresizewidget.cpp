#include "ttkabstractmoveresizewidget.h"
#include "ttklibrary.h"

#include <QApplication>

static constexpr int DISTANCE = 5;

#if !TTK_QT_VERSION_CHECK(5,0,0) && defined(Q_OS_UNIX)
#  define GEOMETRY(x, y, w, h) move(x, y); resize(w, h);
#else
#  define GEOMETRY(x, y, w, h) setGeometry(x, y, w, h);
#endif

TTKAbstractMoveResizeWidget::TTKAbstractMoveResizeWidget(QWidget *parent)
    : TTKAbstractMoveResizeWidget(true, parent)
{

}

TTKAbstractMoveResizeWidget::TTKAbstractMoveResizeWidget(bool transparent, QWidget *parent)
    : QWidget(parent),
      m_direction(TTK::Direction::No)
{
    m_struct.m_mouseLeftPress = false;
    m_struct.m_isPressBorder = false;

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, transparent);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);
}

bool TTKAbstractMoveResizeWidget::eventFilter(QObject *object, QEvent *event)
{
    QWidget::eventFilter(object, event);
    if(QEvent::MouseMove == event->type())
    {
        QMouseEvent *mouseEvent = TTKStaticCast(QMouseEvent*, event);
        QApplication::sendEvent(this, mouseEvent);
    }
    return false;
}

void TTKAbstractMoveResizeWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if(m_struct.m_isPressBorder || m_direction == TTK::Direction::No)
    {
        return;
    }

    const QPoint &point = mapFromGlobal(QCursor::pos());
    if(point.y() > DISTANCE && point.y() < height() - DISTANCE && point.x() > DISTANCE && point.x() < width() - DISTANCE)
    {
        setCursor(Qt::ArrowCursor);
        m_direction = TTK::Direction::No;
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
            m_struct.m_mousePos = QtMouseGlobalPos(event);
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
        move(m_struct.m_windowPos + QtMouseGlobalPos(event) - m_struct.m_mousePos);
    }
}

void TTKAbstractMoveResizeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_struct.m_isPressBorder = false;
    m_struct.m_mouseLeftPress = false;
    setCursor(QCursor(Qt::ArrowCursor));
    m_direction = TTK::Direction::No;
}

void TTKAbstractMoveResizeWidget::sizeDirection()
{
    const QPoint &point = mapFromGlobal(QCursor::pos());
    if(point.x() < DISTANCE && point.y() < height() - DISTANCE && point.y() > DISTANCE)
    {
        setCursor(Qt::SizeHorCursor);
        m_direction = TTK::Direction::Left;
    }
    else if(point.x() > width() - DISTANCE && point.y() < height() - DISTANCE && point.y() > DISTANCE)
    {
        setCursor(Qt::SizeHorCursor);
        m_direction = TTK::Direction::Right;
    }
    else if(point.y() < DISTANCE && point.x() > DISTANCE && point.x() < width() - DISTANCE)
    {
        setCursor(Qt::SizeVerCursor);
        m_direction = TTK::Direction::Top;
    }
    else if(point.y() > height() - DISTANCE && point.x() > DISTANCE && point.x() < width() - DISTANCE)
    {
        setCursor(Qt::SizeVerCursor);
        m_direction = TTK::Direction::Bottom;
    }
    else if(point.y() < DISTANCE && point.x() < DISTANCE)
    {
        setCursor(Qt::SizeFDiagCursor);
        m_direction = TTK::Direction::LeftTop;
    }
    else if(point.y() < DISTANCE && point.x() > width() - DISTANCE)
    {
        setCursor(Qt::SizeBDiagCursor);
        m_direction = TTK::Direction::RightTop;
    }
    else if(point.x() < DISTANCE && point.y() > height() - DISTANCE)
    {
        setCursor(Qt::SizeBDiagCursor);
        m_direction = TTK::Direction::LeftBottom;
    }
    else if(point.x() > DISTANCE && point.y() > height() - DISTANCE)
    {
        setCursor(Qt::SizeFDiagCursor);
        m_direction = TTK::Direction::RightBottom;
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        m_direction = TTK::Direction::No;
    }
}

void TTKAbstractMoveResizeWidget::moveDirection()
{
    const QPoint &point = QCursor::pos();
    switch(m_direction)
    {
        case TTK::Direction::Left:
        {
            const int wValue = x() + width() - point.x();
            if(minimumWidth() <= wValue && wValue <= maximumWidth())
            {
                GEOMETRY(point.x(), y(), wValue, height());
            }
            break;
        }
        case TTK::Direction::Right:
        {
            const int wValue = point.x() - x();
            if(minimumWidth() <= wValue && wValue <= maximumWidth())
            {
                GEOMETRY(x(), y(), wValue, height());
            }
            break;
        }
        case TTK::Direction::Top:
        {
            const int hValue = y() - point.y() + height();
            if(minimumHeight() <= hValue && hValue <= maximumHeight())
            {
                GEOMETRY(x(), point.y(), width(), hValue);
            }
            break;
        }
        case TTK::Direction::Bottom:
        {
            const int hValue = point.y() - y();
            if(minimumHeight() <= hValue && hValue <= maximumHeight())
            {
                GEOMETRY(x(), y(), width(), hValue);
            }
            break;
        }
        case TTK::Direction::LeftTop:
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

            GEOMETRY(xValue, yValue, wValue, hValue);
            break;
        }
        case TTK::Direction::RightTop:
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

            GEOMETRY(m_struct.m_windowPos.x(), yValue, wValue, hValue);
            break;
        }
        case TTK::Direction::LeftBottom:
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

            GEOMETRY(xValue, m_struct.m_windowPos.y(), wValue, hValue);
            break;
        }
        case TTK::Direction::RightBottom:
        {
            const int wValue = point.x() - x();
            const int hValue = point.y() - y();
            GEOMETRY(m_struct.m_windowPos.x(), m_struct.m_windowPos.y(), wValue, hValue);
            break;
        }
        default: break;
    }
}

void TTKAbstractMoveResizeWidget::setObjectsTracking(const QWidgetList &objects)
{
    for(QWidget *object : objects)
    {
        object->installEventFilter(this);
        object->setMouseTracking(true);
    }
}
