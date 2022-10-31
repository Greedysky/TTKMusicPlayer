#include "ttkclickedlabel.h"

#include <QMouseEvent>

TTKClickedLabel::TTKClickedLabel(QWidget *parent)
    : QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
}

TTKClickedLabel::TTKClickedLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    setCursor(Qt::PointingHandCursor);
}

void TTKClickedLabel::mousePressEvent(QMouseEvent *event)
{
//    QLabel::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
       Q_EMIT clicked();
    }
}
