#include "musiclrcartphotolabel.h"

#include <qmath.h>
#include <QPainter>
#include <QWheelEvent>

MusicLrcArtPhotoLabel::MusicLrcArtPhotoLabel(QWidget *parent)
    : QWidget(parent)
{
    m_width = 0;
    m_height = 0;
    m_ratio = 0.0f;
    m_picMoved = false;
}

QString MusicLrcArtPhotoLabel::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcArtPhotoLabel::setImagePath(const QString &path)
{
    m_path = path;
    m_showPix.load(path);
    m_width = m_showPix.width();
    m_height = m_showPix.height();
    m_ratio = m_width*1.0/m_height;
}

void MusicLrcArtPhotoLabel::saveImagePath(const QString &path) const
{
    QPixmap px(m_width, m_height);
    QPainter paint(&px);
    paint.drawPixmap(0, 0, m_width, m_height, m_showPix);
    paint.end();
    px.save(path);
}

void MusicLrcArtPhotoLabel::paintEvent(QPaintEvent *event)
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

    m_imagePos.setX(m_imagePos.x() + m_deltaPos.x());
    m_imagePos.setY(m_imagePos.y() + m_deltaPos.y());
    painter.drawPixmap(m_imagePos.x(), m_imagePos.y(), m_width, m_height, m_showPix);
    painter.end();
}

void MusicLrcArtPhotoLabel::wheelEvent(QWheelEvent *event)
{
    m_deltaPos = QPoint();
    QWidget::wheelEvent(event);
    float delta = event->delta() / 100.0;

    m_width = delta > 0 ?  m_width * delta : m_width * (1 / -delta);
    m_height = delta > 0 ?  m_height * delta : m_height * (1 / -delta);

    if(m_width < m_height && m_width < 10)
    {
        m_width = 10;
        m_height = m_width/m_ratio;
    }
    else if(m_width > m_height && m_height < 10)
    {
        m_height = 10;
        m_width = m_height*m_ratio;
    }

    m_showPix.scaled(m_width, m_height, Qt::KeepAspectRatio);
    update();
}

void MusicLrcArtPhotoLabel::mousePressEvent(QMouseEvent *event)
{
//    QWidget::mousePressEvent(event);
    if(event->button() == Qt::MiddleButton ||
       event->button() == Qt::LeftButton )
    {
        m_picMoved = true;
        m_pressedPos = event->pos();
    }
}

void MusicLrcArtPhotoLabel::mouseMoveEvent(QMouseEvent *event)
{
//    QWidget::mouseMoveEvent(event);
    if(m_picMoved)
    {
        m_deltaPos = event->pos() - m_pressedPos;
        m_pressedPos = event->pos();
        update();
    }
}

void MusicLrcArtPhotoLabel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
//    QWidget::mouseReleaseEvent(event);
    m_picMoved = false;
}

void MusicLrcArtPhotoLabel::mapRGBTableToPixmap()
{
    QImage image = m_showPix.toImage();
    for(int i=0; i<image.height(); i++)
    {
        uchar* out = image.scanLine(i);
        for(int j=0; j<image.width(); j++)
        {
            *out = mapValueToColor(*out); //R
            ++out;
            *out = mapValueToColor(*out); //G
            ++out;
            *out = mapValueToColor(*out); //B
            ++out;
            *out++ = 0xff; //A
        }
    }
    m_showPix = QPixmap::fromImage(image);
}

uchar MusicLrcArtPhotoLabel::mapValueToColor(float value)
{
    int x = m_deltaPos.x();
    int y = m_deltaPos.y();

    float color = (value - x)/y *255.0 + 127.5 ;
    if(color < 0.0)
    {
        color = 0.0;
    }
    else if(color > 255.0)
    {
        color = 255.0;
    }
    uchar ucolor = (uchar)roundf(color);
    return ucolor;
}
