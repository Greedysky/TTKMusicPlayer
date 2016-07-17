#include "musicclickedlabel.h"

#include <QMouseEvent>

MusicClickedLabel::MusicClickedLabel(QWidget *parent)
    : QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
}

QString MusicClickedLabel::getClassName()
{
    return staticMetaObject.className();
}

void MusicClickedLabel::mousePressEvent(QMouseEvent *event)
{
//    QLabel::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
       emit clicked();
    }
}
