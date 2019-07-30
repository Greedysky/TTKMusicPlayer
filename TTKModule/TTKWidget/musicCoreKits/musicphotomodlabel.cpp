#include "musicphotomodlabel.h"

#include <qmath.h>
#include <QPainter>
#include <QWheelEvent>

MusicPhotoModLabel::MusicPhotoModLabel(QWidget *parent)
    : QWidget(parent)
{
    m_width = 0;
    m_height = 0;
    m_originWidth = 0;
    m_ratio = 0.0f;
    m_picMoved = false;
}

void MusicPhotoModLabel::setImagePath(const QString &path)
{
    m_path = path;
    m_showPix.load(path);
    m_width = m_showPix.width();
    m_height = m_showPix.height();
    m_originWidth = m_width;
    m_ratio = m_width*1.0/m_height;
}

void MusicPhotoModLabel::saveImagePath(const QString &path) const
{
    QPixmap px(m_width, m_height);
    QPainter paint(&px);
    paint.drawPixmap(0, 0, m_width, m_height, m_showPix);
    paint.end();
    px.save(path);
}

void MusicPhotoModLabel::paintEvent(QPaintEvent *event)
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

    m_imagePos.setX(m_imagePos.x() + m_deltaPos.x());
    m_imagePos.setY(m_imagePos.y() + m_deltaPos.y());

    const QRect imageRect(m_imagePos.x(), m_imagePos.y(), m_width, m_height);
    painter.drawPixmap(imageRect, m_showPix);
}

void MusicPhotoModLabel::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);
    m_deltaPos = QPoint();

    float delta = event->delta() / 100.0;
    float dv = m_width*1.0/m_originWidth;
    if(dv >= 5 && delta >= 0)
    {
        return;
    }

    m_width = delta >= 0 ?  m_width * delta : m_width * (1 / -delta);
    m_height = delta >= 0 ?  m_height * delta : m_height * (1 / -delta);

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

    dv = m_width*1.0/m_originWidth;
    emit deltaValueChanged(dv);

    m_showPix.scaled(m_width, m_height, Qt::KeepAspectRatio);
    update();
}

void MusicPhotoModLabel::mousePressEvent(QMouseEvent *event)
{
//    QWidget::mousePressEvent(event);
    if(event->button() == Qt::MiddleButton || event->button() == Qt::LeftButton )
    {
        m_picMoved = true;
        m_pressedPos = event->pos();
    }
}

void MusicPhotoModLabel::mouseMoveEvent(QMouseEvent *event)
{
//    QWidget::mouseMoveEvent(event);
    if(m_picMoved)
    {
        m_deltaPos = event->pos() - m_pressedPos;
        m_pressedPos = event->pos();
        update();
    }
}

void MusicPhotoModLabel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
//    QWidget::mouseReleaseEvent(event);
    m_picMoved = false;
}
