#include "musicmarquee.h"
#include <QtGui>

MusicMarquee::MusicMarquee(QWidget *parent)
    : QWidget(parent)
{
    m_offset = 0;
    m_myTimerId = 0;
}

void MusicMarquee::setText(const QString &newText)
{
    m_myText = newText.leftJustified(50,' ');
    update();
    updateGeometry();
}

QSize MusicMarquee::sizeHint() const
{
    return fontMetrics().size(0, text());
}

void MusicMarquee::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int textWidth = fontMetrics().width(text());
    if(textWidth < 1)
    {
        return;
    }
    int x = -m_offset;
    while(x < width())
    {
        painter.drawText(x, 0, textWidth, height(),
                         Qt::AlignLeft | Qt::AlignVCenter, text());
        x += textWidth;
    }
    painter.end();
}

void MusicMarquee::showEvent(QShowEvent *)
{
    m_myTimerId = startTimer(30);
}

void MusicMarquee::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_myTimerId)
    {
        ++m_offset;
        if (m_offset >= fontMetrics().width(text()))
            m_offset = 0;
        scroll(-1, 0);
    }
    else
    {
        QWidget::timerEvent(event);
    }
}

void MusicMarquee::hideEvent(QHideEvent *)
{
    killTimer(m_myTimerId);
    m_myTimerId = 0;
}
