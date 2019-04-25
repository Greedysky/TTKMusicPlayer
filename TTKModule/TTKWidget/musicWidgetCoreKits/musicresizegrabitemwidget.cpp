#include "musicresizegrabitemwidget.h"

#include <qmath.h>
#include <QPainter>
#include <QMouseEvent>

#define PADDING          6

MusicResizeGrabItemWidget::MusicResizeGrabItemWidget(QWidget *parent)
    : QWidget(parent)
{
    m_direction = Direction_No;
    m_isPressed = false;
    m_geometricStretch = false;
    m_crossStretch = true;

    setMouseTracking(true);
}

void MusicResizeGrabItemWidget::setBorderRect(const QRect &rect)
{
    m_borderRect = rect - QMargins(1, 1, 1, 1);
}

void MusicResizeGrabItemWidget::onMouseChange(int x, int y)
{
    if(x < 0 || y < 0)
    {
        return;
    }

    const int rx = (x >= m_originPoint.x()) ? m_originPoint.x() : x;
    const int ry = (y >= m_originPoint.y()) ? m_originPoint.y() : y;
    int rw = abs(x - m_originPoint.x());
    int rh = abs(y - m_originPoint.y());

    if(!m_borderRect.contains(QRect(rx, ry, rw, rh)))
    {
        return;
    }

    if(m_geometricStretch && m_direction != Direction_No)
    {
        switch(m_direction)
        {
            case Direction_Left:
            case Direction_Right:
//            case Direction_LeftTop:
//            case Direction_LeftBottom:
                rh = rw; break;
            case Direction_Top:
            case Direction_Bottom:
//            case Direction_RightTop:
//            case Direction_RightBottom:
                rw = rh; break;
            default: break;
        }
    }

    m_currentRect = QRect(rx, ry, rw, rh);
    setGeometry(m_currentRect);
}

void MusicResizeGrabItemWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        if(m_direction != Direction_No)
        {
            mouseGrabber();
        }
        m_movePos = event->globalPos() - pos();
    }
}

void MusicResizeGrabItemWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = false;
        if(m_direction != Direction_No)
        {
            setCursor(QCursor(Qt::SizeAllCursor));
        }

        emit rectChanged();
    }
}

void MusicResizeGrabItemWidget::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint &gloPoint = mapToParent(event->pos());

    const QPoint &pt_lu = mapToParent(rect().topLeft());
    const QPoint &pt_ll = mapToParent(rect().bottomLeft());
    const QPoint &pt_rl = mapToParent(rect().bottomRight());
    const QPoint &pt_ru = mapToParent(rect().topRight());

    if(!m_isPressed)
    {
        m_direction = getRegion(gloPoint);
        switch(m_direction)
        {
            case Direction_No:
            case Direction_Right:
            case Direction_RightBottom:
                m_originPoint = pt_lu;
                break;
            case Direction_RightTop:
                m_originPoint = pt_ll;
                break;
            case Direction_Left:
            case Direction_LeftBottom:
                m_originPoint = pt_ru;
                break;
            case Direction_LeftTop:
            case Direction_Top:
                m_originPoint = pt_rl;
                break;
            case Direction_Bottom:
                m_originPoint = pt_lu;
                break;
            default: break;
        }
    }
    else
    {
        if(m_direction != Direction_No)
        {
            switch(m_direction)
            {
                case Direction_Left:
                    return onMouseChange(gloPoint.x(), pt_ll.y() + 1);
                case Direction_Right:
                    return onMouseChange(gloPoint.x(), pt_rl.y() + 1);
                case Direction_Top:
                    return onMouseChange(pt_lu.x(), gloPoint.y());
                case Direction_Bottom:
                    return onMouseChange(pt_rl.x() + 1, gloPoint.y());
                case Direction_LeftTop:
                case Direction_RightTop:
                case Direction_LeftBottom:
                case Direction_RightBottom:
                    if(m_crossStretch)
                    {
                        return onMouseChange(gloPoint.x(), gloPoint.y());
                    }
                    else
                    {
                        break;
                    }
                default: break;
            }
        }
        else
        {
            if(!m_borderRect.contains(QRect(event->globalPos() - m_movePos, size())))
            {
                return;
            }

            move(event->globalPos() - m_movePos);
            m_movePos = event->globalPos() - pos();
        }
    }
    m_currentRect = geometry();
}

void MusicResizeGrabItemWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    m_listMarker.clear();
    m_listMarker.push_back(QPoint(0, 0));
    m_listMarker.push_back(QPoint(width(), 0));
    m_listMarker.push_back(QPoint(0, height()));
    m_listMarker.push_back(QPoint(width(), height()));

    m_listMarker.push_back(QPoint((width() >> 1), 0));
    m_listMarker.push_back(QPoint((width() >> 1), height()));
    m_listMarker.push_back(QPoint(0, (height() >> 1)));
    m_listMarker.push_back(QPoint(width(), (height() >> 1)));
}

void MusicResizeGrabItemWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    QPen pen(QColor(0, 174, 255), 2);
    painter.setPen(pen);
    painter.drawRect(rect());

    pen.setWidth(4);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoints(m_listMarker);
}

MusicResizeGrabItemWidget::Direction MusicResizeGrabItemWidget::getRegion(const QPoint &cursor)
{
    Direction ret_dir = Direction_No;

    const QPoint &pt_lu = mapToParent(rect().topLeft());
    const QPoint &pt_rl = mapToParent(rect().bottomRight());

    const int x = cursor.x();
    const int y = cursor.y();

    if(pt_lu.x() + PADDING >= x && pt_lu.x() <= x && pt_lu.y() + PADDING >= y && pt_lu.y() <= y)
    {
        ret_dir = Direction_LeftTop;
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x >= pt_rl.x() - PADDING && x <= pt_rl.x() && y >= pt_rl.y() - PADDING && y <= pt_rl.y())
    {
        ret_dir = Direction_RightBottom;
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x <= pt_lu.x() + PADDING && x >= pt_lu.x() && y >= pt_rl.y() - PADDING && y <= pt_rl.y())
    {
        ret_dir = Direction_LeftBottom;
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x <= pt_rl.x() && x >= pt_rl.x() - PADDING && y >= pt_lu.y() && y <= pt_lu.y() + PADDING)
    {
        ret_dir = Direction_RightTop;
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x <= pt_lu.x() + PADDING && x >= pt_lu.x())
    {
        ret_dir = Direction_Left;
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(x <= pt_rl.x() && x >= pt_rl.x() - PADDING)
    {
        ret_dir = Direction_Right;
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(y >= pt_lu.y() && y <= pt_lu.y() + PADDING)
    {
        ret_dir = Direction_Top;
        setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if(y <= pt_rl.y() && y >= pt_rl.y() - PADDING)
    {
        ret_dir = Direction_Bottom;
        setCursor(QCursor(Qt::SizeVerCursor));
    }
    else
    {
        ret_dir = Direction_No;
        setCursor(QCursor(Qt::SizeAllCursor));
    }

    return ret_dir;
}

#if QT_VERSION < QT_VERSION_CHECK(5,3,0)
QRect operator-(const QRect &rect, const QMargins &margins)
{
    return QRect(QPoint(rect.left() + margins.left(), rect.top() + margins.top()),
                 QPoint(rect.right() - margins.right(), rect.bottom() - margins.bottom()));
}
#endif
