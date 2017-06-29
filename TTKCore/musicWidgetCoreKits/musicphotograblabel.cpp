#include "musicphotograblabel.h"

#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

const int PADDING = 6;

MusicPhotoGrabItem::MusicPhotoGrabItem(QWidget *parent)
    : QWidget(parent)
{
    m_direction = NONE;
    m_isPressed = false;

    setMouseTracking(true);
}

QString MusicPhotoGrabItem::getClassName()
{
    return staticMetaObject.className();
}

void MusicPhotoGrabItem::setBorderRect(const QRect &rect)
{
    m_borderRect = rect - QMargins(2, 2, 2, 2);
    move(m_borderRect.topLeft());
    resize(m_borderRect.width(), m_borderRect.width());
}

void MusicPhotoGrabItem::onMouseChange(int x, int y)
{
    if(x < 0 || y < 0)
    {
        return;
    }

    const int rx = (x >= m_originPoint.x()) ? m_originPoint.x() : x;
    const int ry = (y >= m_originPoint.y()) ? m_originPoint.y() : y;
    const int rw = abs(x - m_originPoint.x());
    const int rh = abs(y - m_originPoint.y());

    m_currentRect = QRect(rx, ry, rw, rh);
    setGeometry(m_currentRect);
}

void MusicPhotoGrabItem::mousePressEvent(QMouseEvent *event)
{
//    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        if(m_direction != NONE)
        {
            mouseGrabber();
        }
        m_movePos = event->globalPos() - pos();
    }
}

void MusicPhotoGrabItem::mouseReleaseEvent(QMouseEvent *event)
{
//    QWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = false;
        if(m_direction != NONE)
        {
            setCursor(QCursor(Qt::SizeAllCursor));
        }

        emit rectChanged();
    }
}

void MusicPhotoGrabItem::mouseMoveEvent(QMouseEvent *event)
{
//    QWidget::mouseMoveEvent(event);
    QPoint gloPoint = mapToParent(event->pos());
    // left upper
    QPoint pt_lu = mapToParent(rect().topLeft());
    // left lower
    QPoint pt_ll = mapToParent(rect().bottomLeft());
    // right lower
    QPoint pt_rl = mapToParent(rect().bottomRight());
    // right upper
    QPoint pt_ru = mapToParent(rect().topRight());

    if(!m_isPressed)
    {
        m_direction = getRegion(gloPoint);
        switch(m_direction)
        {
            case NONE:
            case RIGHT:
            case RIGHTLOWER:
                m_originPoint = pt_lu;
                break;
            case RIGHTUPPER:
                m_originPoint = pt_ll;
                break;
            case LEFT:
            case LEFTLOWER:
                m_originPoint = pt_ru;
                break;
            case LEFTUPPER:
            case UPPER:
                m_originPoint = pt_rl;
                break;
            case LOWER:
                m_originPoint = pt_lu;
                break;
        }
    }
    else
    {
        if(m_direction != NONE)
        {
            const int global_x = gloPoint.x();
            switch(m_direction)
            {
                case LEFT:
                    return onMouseChange(global_x, pt_ll.y() + 1);
                case RIGHT:
                    return onMouseChange(global_x, pt_rl.y() + 1);
                case UPPER:
                    return onMouseChange(pt_lu.x(), gloPoint.y());
                case LOWER:
                    return onMouseChange(pt_rl.x() + 1, gloPoint.y());
                case LEFTUPPER:
                case RIGHTUPPER:
                case LEFTLOWER:
                case RIGHTLOWER:
                    return onMouseChange(global_x, gloPoint.y());
                default:
                    break;
            }
        }
        else
        {
            move(event->globalPos() - m_movePos);
            m_movePos = event->globalPos() - pos();
        }
    }
    m_currentRect = geometry();
}

void MusicPhotoGrabItem::resizeEvent(QResizeEvent *event)
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

void MusicPhotoGrabItem::paintEvent(QPaintEvent *event)
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

MusicPhotoGrabItem::DIRECTION MusicPhotoGrabItem::getRegion(const QPoint &cursor)
{
    MusicPhotoGrabItem::DIRECTION ret_dir = NONE;
    // left upper
    QPoint pt_lu = mapToParent(rect().topLeft());
    // right lower
    QPoint pt_rl = mapToParent(rect().bottomRight());

    int x = cursor.x();
    int y = cursor.y();

    if(pt_lu.x() + PADDING >= x && pt_lu.x() <= x &&
       pt_lu.y() + PADDING >= y && pt_lu.y() <= y)
    {
        ret_dir = LEFTUPPER;
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x >= pt_rl.x() - PADDING && x <= pt_rl.x() &&
            y >= pt_rl.y() - PADDING && y <= pt_rl.y())
    {
        ret_dir = RIGHTLOWER;
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x <= pt_lu.x() + PADDING && x >= pt_lu.x() &&
            y >= pt_rl.y() - PADDING && y <= pt_rl.y())
    {
        ret_dir = LEFTLOWER;
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x <= pt_rl.x() && x >= pt_rl.x() - PADDING &&
            y >= pt_lu.y() && y <= pt_lu.y() + PADDING)
    {
        ret_dir = RIGHTUPPER;
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x <= pt_lu.x() + PADDING && x >= pt_lu.x())
    {
        ret_dir = LEFT;
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(x <= pt_rl.x() && x >= pt_rl.x() - PADDING)
    {
        ret_dir = RIGHT;
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(y >= pt_lu.y() && y <= pt_lu.y() + PADDING)
    {
        ret_dir = UPPER;
        setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if(y <= pt_rl.y() && y >= pt_rl.y() - PADDING)
    {
        ret_dir = LOWER;
        setCursor(QCursor(Qt::SizeVerCursor));
    }
    else
    {
        ret_dir = NONE;
        setCursor(QCursor(Qt::SizeAllCursor));
    }

    return ret_dir;
}



MusicPhotoGrabLabel::MusicPhotoGrabLabel(QWidget *parent)
    : QWidget(parent)
{
    m_ratio = 1.0f;
    m_grabItem = new MusicPhotoGrabItem(this);
    connect(m_grabItem, SIGNAL(rectChanged()), SLOT(rectChanged()));
    m_grabItem->hide();
}

MusicPhotoGrabLabel::~MusicPhotoGrabLabel()
{
    delete m_grabItem;
}

QString MusicPhotoGrabLabel::getClassName()
{
    return staticMetaObject.className();
}

void MusicPhotoGrabLabel::setImagePath(const QString &path)
{
    m_path = path;
    QPixmap img(m_path);

    if(img.width() > img.height())
    {
        m_ratio = img.width()*1.0f/width();
        m_imgRect = QRect(0, 0, img.width()/m_ratio, img.height()/m_ratio);
    }
    else
    {
        m_ratio = img.height()*1.0f/height();
        m_imgRect = QRect(0, 0, img.width()/m_ratio, img.height()/m_ratio);
    }
    m_imgRect.setX((width() - m_imgRect.width())/2);
    m_imgRect.setY((height() - m_imgRect.height())/2);

    m_grabItem->setBorderRect(m_imgRect);
    m_grabItem->show();
}

void MusicPhotoGrabLabel::rectChanged()
{
    QPixmap img(m_path);
    emit intersectedPixmap(img.copy(
            QRect((m_grabItem->geometry().topLeft() - m_imgRect.topLeft())*m_ratio,
                   m_grabItem->geometry().size()*m_ratio)).scaled(100, 100));
}

void MusicPhotoGrabLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    QPixmap img(m_path);
    painter.drawPixmap(m_imgRect, img);
    painter.end();
}
