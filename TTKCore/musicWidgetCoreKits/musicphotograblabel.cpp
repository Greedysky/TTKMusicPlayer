#include "musicphotograblabel.h"

#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

#define PADDING          6
#define PIX_HEIGHT      16
#define PIX_WIDTH       16

MusicPhotoGrabItem::MusicPhotoGrabItem(QWidget *parent)
    : QWidget(parent)
{
    m_direction = Direction_No;
    m_isPressed = false;

    setMouseTracking(true);
}

QString MusicPhotoGrabItem::getClassName()
{
    return staticMetaObject.className();
}

void MusicPhotoGrabItem::setBorderRect(const QRect &rect)
{
    m_borderRect = rect - QMargins(1, 1, 1, 1);
    move(m_borderRect.topLeft());
    int v = (m_borderRect.width() < m_borderRect.height()) ?
             m_borderRect.width() : m_borderRect.height();
    resize(v, v);
}

void MusicPhotoGrabItem::onMouseChange(int x, int y)
{
    if(x < 0 || y < 0)
    {
        return;
    }

    int rx = (x >= m_originPoint.x()) ? m_originPoint.x() : x;
    int ry = (y >= m_originPoint.y()) ? m_originPoint.y() : y;
    int rw = abs(x - m_originPoint.x());
    int rh = abs(y - m_originPoint.y());

    if(!m_borderRect.contains(QRect(rx, ry, rw, rh)))
    {
        return;
    }

    if(m_direction != Direction_No)
    {
        switch(m_direction)
        {
            case Direction_Left:
            case Direction_Right:
            case Direction_LeftTop:
            case Direction_LeftBottom:
                rh = rw; break;
            case Direction_Top:
            case Direction_Bottom:
            case Direction_RightTop:
            case Direction_RightBottom:
                rw = rh; break;
            default:
                break;
        }
    }

    m_currentRect = QRect(rx, ry, rw, rh);
    setGeometry(m_currentRect);
}

void MusicPhotoGrabItem::mousePressEvent(QMouseEvent *event)
{
//    QWidget::mousePressEvent(event);
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

void MusicPhotoGrabItem::mouseReleaseEvent(QMouseEvent *event)
{
//    QWidget::mouseReleaseEvent(event);
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
                    break;
//                    return onMouseChange(gloPoint.x(), gloPoint.y());
                default:
                    break;
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

MusicPhotoGrabItem::Direction MusicPhotoGrabItem::getRegion(const QPoint &cursor)
{
    Direction ret_dir = Direction_No;
    // left upper
    QPoint pt_lu = mapToParent(rect().topLeft());
    // right lower
    QPoint pt_rl = mapToParent(rect().bottomRight());

    int x = cursor.x();
    int y = cursor.y();

    if(pt_lu.x() + PADDING >= x && pt_lu.x() <= x &&
       pt_lu.y() + PADDING >= y && pt_lu.y() <= y)
    {
        ret_dir = Direction_LeftTop;
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x >= pt_rl.x() - PADDING && x <= pt_rl.x() &&
            y >= pt_rl.y() - PADDING && y <= pt_rl.y())
    {
        ret_dir = Direction_RightBottom;
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x <= pt_lu.x() + PADDING && x >= pt_lu.x() &&
            y >= pt_rl.y() - PADDING && y <= pt_rl.y())
    {
        ret_dir = Direction_LeftBottom;
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x <= pt_rl.x() && x >= pt_rl.x() - PADDING &&
            y >= pt_lu.y() && y <= pt_lu.y() + PADDING)
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
    update();
}

QPixmap MusicPhotoGrabLabel::pixmap()
{
    QPixmap img(m_path);
    return img.copy(
            QRect((m_grabItem->geometry().topLeft() - m_imgRect.topLeft())*m_ratio,
                   m_grabItem->geometry().size()*m_ratio));
}

void MusicPhotoGrabLabel::rectChanged()
{
    emit intersectedPixmap( pixmap() );
}

void MusicPhotoGrabLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QPixmap bgPix(":/lrc/lb_transparent");
    for(int i=0; i<ceil(width()/PIX_WIDTH); ++i)
    {
        for(int j=0; j<=ceil(height()/PIX_HEIGHT); ++j)
        {
            painter.drawPixmap(i*PIX_WIDTH, j*PIX_HEIGHT, PIX_WIDTH, PIX_HEIGHT, bgPix);
        }
    }

    QPixmap img(m_path);
    painter.drawPixmap(m_imgRect, img);
    painter.end();
}
