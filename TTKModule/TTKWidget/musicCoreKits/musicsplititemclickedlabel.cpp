#include "musicsplititemclickedlabel.h"
#include "musicrightareawidget.h"
#include "musicwidgetutils.h"

#include <QMouseEvent>

MusicSplitItemClickedLabel::MusicSplitItemClickedLabel(QWidget *parent)
    : QLabel(parent)
{
    setAlignment(Qt::AlignVCenter);
    setMouseTracking(true);
}

void MusicSplitItemClickedLabel::enterEvent(QtEnterEvent *event)
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
        MusicRightAreaWidget::instance()->showSongSearchedFound(m_currentString);
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
        const int fs = TTK::Widget::fontTextWidth(font(), var.trimmed());
        if(offset <= QtPositionX(event) && QtPositionX(event) <= offset + fs)
        {
            setCursor(QCursor(Qt::PointingHandCursor));
            m_lineGeometry = QRectF(offset, height() - 3, fs, 0.5);
            m_currentString = var.trimmed();
            break;
        }
        offset += (fs + TTK::Widget::fontTextWidth(font(), " - "));
    }
    update();
}

void MusicSplitItemClickedLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(m_lineGeometry);
}
