#include "musiclrcartphotolabel.h"

#include <qmath.h>
#include <QPainter>
#include <QWheelEvent>

MusicLrcArtPhotoLabel::MusicLrcArtPhotoLabel(QWidget *parent)
    : QWidget(parent)
{
    m_width = 0;
    m_height = 0;
}

void MusicLrcArtPhotoLabel::setImagePath(const QString &path)
{
    m_path = path;
    m_showPix.load(path);
    m_width = m_showPix.width();
    m_height = m_showPix.height();
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
    QPixmap bgPix(":/image/imagetrans");
    for(int i=0; i<ceil(width()/PIX_WIDTH); ++i)
    {
        for(int j=0; j<ceil(height()/PIX_HEIGHT); ++j)
        {
            painter.drawPixmap(i*PIX_WIDTH, j*PIX_HEIGHT, PIX_WIDTH, PIX_HEIGHT, bgPix);
        }
    }

    painter.drawPixmap(0, 0, m_width, m_height, m_showPix);
    painter.end();
}

void MusicLrcArtPhotoLabel::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);
    float delta = event->delta() / 100.0;

    m_width = delta > 0 ?  m_width * delta : m_width * (1 / -delta);
    m_height = delta > 0 ?  m_height * delta : m_height * (1 / -delta);

    m_showPix.scaled(m_width, m_height, Qt::KeepAspectRatio);
    update();
}
