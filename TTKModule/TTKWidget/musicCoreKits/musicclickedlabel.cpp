#include "musicclickedlabel.h"

#include <QMouseEvent>

MusicClickedLabel::MusicClickedLabel(QWidget *parent)
    : QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
}

MusicClickedLabel::MusicClickedLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    setCursor(Qt::PointingHandCursor);
}

void MusicClickedLabel::mousePressEvent(QMouseEvent *event)
{
//    QLabel::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
       emit clicked();
    }
}
