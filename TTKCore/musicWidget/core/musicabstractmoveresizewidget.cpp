#include "musicabstractmoveresizewidget.h"
#include "musicsettingmanager.h"

#include <QMouseEvent>
#include <QApplication>

#define DISTANCE    5

MusicAbstractMoveResizeWidget::MusicAbstractMoveResizeWidget(QWidget *parent)
    : QWidget(parent)
{
    m_mouseLeftPress = false;
    m_direction = Direction_No;

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
}

MusicAbstractMoveResizeWidget::~MusicAbstractMoveResizeWidget()
{

}

QString MusicAbstractMoveResizeWidget::getClassName()
{
    return staticMetaObject.className();
}

bool MusicAbstractMoveResizeWidget::eventFilter(QObject *object, QEvent *event)
{
    QWidget::eventFilter(object, event);
    if(QEvent::MouseMove == event->type())
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QApplication::sendEvent(this, mouseEvent);
    }
    return false;
}

void MusicAbstractMoveResizeWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
//    if( event->buttons() == Qt::LeftButton)
//    {
//        isMaximized() ? showNormal() : showMaximized();
//    }
}

void MusicAbstractMoveResizeWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_mouseLeftPress = true;
        m_direction = sizeDirection(event->globalPos().x(), event->globalPos().y());
        m_pressAt = event->globalPos();
    }
}

void MusicAbstractMoveResizeWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(m_mouseLeftPress)
    {
        int xpos = event->globalX() - m_pressAt.x();
        int ypos = event->globalY() - m_pressAt.y();
        m_pressAt = event->globalPos();
        if(Direction_No == m_direction)
        {
            if(!isMaximized())
            {
                move( x() + xpos, y() + ypos);
            }
        }
        else
        {
            QSize size = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
            if(Direction_Left == m_direction)
            {
                if(width() - xpos <= WINDOW_WIDTH_MIN || width() - xpos >= size.width())
                {
                    return;
                }
                move( x() + xpos, y());
                resize(width() -  xpos, height());
            }
            else if(Direction_Top == m_direction)
            {
                if(height() - ypos <= WINDOW_HEIGHT_MIN || height() - ypos >= size.height())
                {
                    return;
                }
                move( x(), y() + ypos);
                resize(width(), height() - ypos);
            }
            else if(Direction_Right == m_direction)
            {
                if(width() + xpos >= size.width())
                {
                    return;
                }
                resize(width() + xpos, height());
            }
            else if(Direction_Bottom == m_direction)
            {
                if(height() + ypos >= size.height())
                {
                    return;
                }
                resize(width(), height() + ypos);
            }
            else if(Direction_LeftTop == m_direction)
            {
                if(width() - xpos <= WINDOW_WIDTH_MIN || height() - ypos <= WINDOW_HEIGHT_MIN ||
                   width() - xpos >= size.width() || height() - ypos >= size.height())
                {
                    return;
                }
                move( x() + xpos, y() + ypos);
                resize(width() -  xpos, height() - ypos);
            }
            else if(Direction_LeftBottom == m_direction)
            {
                if(width() - xpos <= WINDOW_WIDTH_MIN || width() - xpos >= size.width() ||
                   height() + ypos >= size.height())
                {
                    return;
                }
                move( x() + xpos, y());
                resize(width() -  xpos, height() + ypos);
            }
            else if(Direction_RightTop == m_direction)
            {
                if(height() - ypos <= WINDOW_HEIGHT_MIN || width() + xpos >= size.width() ||
                   height() - ypos >= size.height())
                {
                    return;
                }
                move( x(), y() + ypos);
                resize(width() +  xpos, height() - ypos);
            }
            else if(Direction_RightBottom == m_direction)
            {
                if(width() + xpos >= size.width() || height() + ypos >= size.height())
                {
                    return;
                }
                resize(width() +  xpos, height() + ypos);
            }
        }
    }
    else if(!isMaximized())
    {
        m_direction = sizeDirection(event->globalPos().x(), event->globalPos().y());
        sizeCursor(m_direction);
    }
}

void MusicAbstractMoveResizeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_mouseLeftPress = false;
    m_pressAt = event->globalPos();
    setCursor(QCursor(Qt::ArrowCursor));
}

void MusicAbstractMoveResizeWidget::sizeCursor(int direction)
{
    if(Direction_No == direction)
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
    else if((Direction_Left == direction) || (Direction_Right == direction))
    {
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if((Direction_Top == direction) || (Direction_Bottom == direction))
    {
        setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if((Direction_LeftTop == direction) || (Direction_RightBottom == direction))
    {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if((Direction_LeftBottom == direction) || (Direction_RightTop == direction))
    {
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }
}

int MusicAbstractMoveResizeWidget::sizeDirection(int x, int y)
{
    QRect rect = geometry();
    int left = rect.left(), right = rect.right();
    int top = rect.top(), bottom = rect.bottom();

    int direction = Direction_No;

    if (x <= left + DISTANCE)
    {
        direction += Direction_Left;
    }
    else if (right-DISTANCE <= x)
    {
        direction += Direction_Right;
    }

    if (y <= top + DISTANCE)
    {
        direction += Direction_Top;
    }
    else if (bottom-DISTANCE <= y)
    {
        direction += Direction_Bottom;
    }

    return direction;
}

QObjectList MusicAbstractMoveResizeWidget::foreachWidget(QObject *object)
{
    QObjectList result;
    foreach(QObject *obj, object->children())
    {
        if("QWidget" == QString(obj->metaObject()->className()))
        {
            result.append(obj);
        }
        result += foreachWidget(obj);
    }
    return result;
}
