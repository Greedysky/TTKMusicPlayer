#include "musicsplititemclickedlabel.h"
#include "musicrightareawidget.h"
#include "musicwidgetutils.h"

#include <QPainter>
#include <QMouseEvent>

MusicSplitItemClickedLabel::MusicSplitItemClickedLabel(QWidget *parent)
    : QLabel(parent)
{
    setAlignment(Qt::AlignVCenter);
    setMouseTracking(true);
}

void MusicSplitItemClickedLabel::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    m_lineGeometry = QRectF();
    m_currentString.clear();
    update();
}

void MusicSplitItemClickedLabel::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_lineGeometry = QRectF();
    m_currentString.clear();
    update();
}

void MusicSplitItemClickedLabel::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    if(event->button() == Qt::LeftButton && !m_currentString.isEmpty())
    {
        MusicRightAreaWidget::instance()->musicSongSearchedFound(m_currentString);
    }
}

void MusicSplitItemClickedLabel::mouseMoveEvent(QMouseEvent *event)
{
    QLabel::mouseMoveEvent(event);

    unsetCursor();
    m_lineGeometry = QRectF();
    m_currentString.clear();

    const QStringList data(text().split(" - "));
    int offset = 0;

    for(const QString &var : qAsConst(data))
    {
        const int fs = MusicUtils::Widget::fontTextWidth(font(), var.trimmed());
        if(offset <= event->pos().x() && event->pos().x() <= offset + fs)
        {
            setCursor(QCursor(Qt::PointingHandCursor));
            m_lineGeometry = QRectF(offset, height() - 3, fs, 0.5);
            m_currentString = var.trimmed();
            break;
        }
        offset += (fs + MusicUtils::Widget::fontTextWidth(font(), (" - ")));
    }
    update();
}

void MusicSplitItemClickedLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);
    painter.drawRect(m_lineGeometry);
}
