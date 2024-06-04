#include "ttkclickedlabel.h"

#include <QMouseEvent>

TTKClickedLabel::TTKClickedLabel(QWidget *parent)
    : QLabel(parent)
    , m_blockOption(false)
{
    setCursor(Qt::PointingHandCursor);
}

TTKClickedLabel::TTKClickedLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
    , m_blockOption(false)
{
    setCursor(Qt::PointingHandCursor);
}

void TTKClickedLabel::mousePressEvent(QMouseEvent *event)
{
//    QLabel::mousePressEvent(event);
    if(event->button() == Qt::LeftButton && !m_blockOption)
    {
       Q_EMIT clicked();
    }
}
