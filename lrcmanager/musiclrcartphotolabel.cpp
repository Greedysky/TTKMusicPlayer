#include "musiclrcartphotolabel.h"

#include <QPainter>

MusicLrcArtPhotoLabel::MusicLrcArtPhotoLabel(QWidget *parent)
    : QWidget(parent)
{

}

void MusicLrcArtPhotoLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    QPixmap pix(":/image/imagetrans");
    for(int i=0; i<ceil(width()/PIX_WIDTH); ++i)
        for(int j=0; j<ceil(height()/PIX_HEIGHT); ++j)
        {
            painter.drawPixmap(i*PIX_WIDTH, j*PIX_HEIGHT, PIX_WIDTH, PIX_HEIGHT, pix);
        }
}
