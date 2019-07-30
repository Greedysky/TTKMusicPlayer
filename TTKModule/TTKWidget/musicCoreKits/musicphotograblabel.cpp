#include "musicphotograblabel.h"

#include <qmath.h>
#include <QPixmap>
#include <QPainter>

#define PIX_HEIGHT      16
#define PIX_WIDTH       16

MusicPhotoGrabItem::MusicPhotoGrabItem(QWidget *parent)
    : MusicResizeGrabItemWidget(parent)
{

}

void MusicPhotoGrabItem::setBorderRect(const QRect &rect)
{
    MusicResizeGrabItemWidget::setBorderRect(rect);
    move(m_borderRect.topLeft());
    const int v = (m_borderRect.width() < m_borderRect.height()) ? m_borderRect.width() : m_borderRect.height();
    resize(v, v);
}



MusicPhotoGrabLabel::MusicPhotoGrabLabel(QWidget *parent)
    : QWidget(parent)
{
    m_ratio = 1.0f;
    m_grabItem = new MusicPhotoGrabItem(this);
    m_grabItem->setGeometricStretch(true);
    connect(m_grabItem, SIGNAL(rectChanged()), SLOT(rectChanged()));
    m_grabItem->hide();
}

MusicPhotoGrabLabel::~MusicPhotoGrabLabel()
{
    delete m_grabItem;
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
    return img.copy(QRect((m_grabItem->geometry().topLeft() - m_imgRect.topLeft())*m_ratio, m_grabItem->geometry().size()*m_ratio));
}

void MusicPhotoGrabLabel::rectChanged()
{
    emit intersectedPixmap( pixmap() );
}

void MusicPhotoGrabLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QPixmap pix(":/lrc/lb_transparent");
    for(int i=0; i<ceil(width()/PIX_WIDTH); ++i)
    {
        for(int j=0; j<=ceil(height()/PIX_HEIGHT); ++j)
        {
            painter.drawPixmap(i*PIX_WIDTH, j*PIX_HEIGHT, PIX_WIDTH, PIX_HEIGHT, pix);
        }
    }

    QPixmap img(m_path);
    painter.drawPixmap(m_imgRect, img);
}
