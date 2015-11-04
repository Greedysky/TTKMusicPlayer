#include "musicspectrumwidget.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"

#include <QMenu>

MusicSpectrumWidget::MusicSpectrumWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    drawWindowShadow(false);

    setMinimumHeight(80);
    setMinimumWidth(150);
    m_direction = NONE;
    setMouseTracking(true);
    connect(&m_timer,SIGNAL(timeout()),SLOT(timeout()));

    M_CONNECTION->setValue("MusicSpectrumWidget", this);
    M_CONNECTION->connect("MusicSpectrumWidget", "MusicPlayer");
}

MusicSpectrumWidget::~MusicSpectrumWidget()
{
    M_CONNECTION->disConnect("MusicSpectrumWidget");
}

void MusicSpectrumWidget::timeout()
{
    emit setSpectrum((HWND)winId(), size().width(), size().height());
}

void MusicSpectrumWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(tr("close"), this, SLOT(close()));
    rightClickMenu.exec(QCursor::pos());
}

void MusicSpectrumWidget::show()
{
    m_timer.start(50);
    QWidget::show();
}

void MusicSpectrumWidget::close()
{
    m_timer.stop();
    QWidget::close();
}

void MusicSpectrumWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_leftButtonPress = false;
        if(m_direction != NONE)
        {
            releaseMouse();
            setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void MusicSpectrumWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_leftButtonPress = true;
        if(m_direction != NONE)
        {
            mouseGrabber();
        }
        else
        {
            m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        }
    }
    else
    {
        QWidget::mousePressEvent(event);
    }
}

void MusicSpectrumWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    QPoint tl = mapToGlobal(rect().topLeft());
    QPoint rb = mapToGlobal(rect().bottomRight());

    if(!m_leftButtonPress)
    {
        region(gloPoint);
    }
    else
    {
        if(m_direction != NONE)
        {
            QRect rMove(tl, rb);
            switch(m_direction)
            {
                case LEFT:
                    if(rb.x() - gloPoint.x() <= minimumWidth())
                        rMove.setX(tl.x());
                    else
                        rMove.setX(gloPoint.x());
                    break;
                case RIGHT:
                    rMove.setWidth(gloPoint.x() - tl.x());
                    break;
                case UP:
                    if(rb.y() - gloPoint.y() <= minimumHeight())
                        rMove.setY(tl.y());
                    else
                        rMove.setY(gloPoint.y());
                    break;
                case DOWN:
                    rMove.setHeight(gloPoint.y() - tl.y());
                    break;
                case LEFTTOP:
                    if(rb.x() - gloPoint.x() <= minimumWidth())
                        rMove.setX(tl.x());
                    else
                        rMove.setX(gloPoint.x());
                    if(rb.y() - gloPoint.y() <= minimumHeight())
                        rMove.setY(tl.y());
                    else
                        rMove.setY(gloPoint.y());
                    break;
                case RIGHTTOP:
                    rMove.setWidth(gloPoint.x() - tl.x());
                    rMove.setY(gloPoint.y());
                    break;
                case LEFTBOTTOM:
                    rMove.setX(gloPoint.x());
                    rMove.setHeight(gloPoint.y() - tl.y());
                    break;
                case RIGHTBOTTOM:
                    rMove.setWidth(gloPoint.x() - tl.x());
                    rMove.setHeight(gloPoint.y() - tl.y());
                    break;
                default: break;
            }
            setGeometry(rMove);
        }
        else
        {
            move(event->globalPos() - m_dragPosition);
            event->accept();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void MusicSpectrumWidget::region(const QPoint &cursorGlobalPoint)
{
    QPoint tl = mapToGlobal(rect().topLeft());
    QPoint rb = mapToGlobal(rect().bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y)
    {
        m_direction = LEFTTOP;
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y())
    {
        m_direction = RIGHTBOTTOM;
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y())
    {
        m_direction = LEFTBOTTOM;
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING)
    {
        m_direction = RIGHTTOP;
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x <= tl.x() + PADDING && x >= tl.x())
    {
        m_direction = LEFT;
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if( x <= rb.x() && x >= rb.x() - PADDING)
    {
        m_direction = RIGHT;
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(y >= tl.y() && y <= tl.y() + PADDING)
    {
        m_direction = UP;
        setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if(y <= rb.y() && y >= rb.y() - PADDING)
    {
        m_direction = DOWN;
        setCursor(QCursor(Qt::SizeVerCursor));
    }
    else
    {
        m_direction = NONE;
        setCursor(QCursor(Qt::ArrowCursor));
    }
}
