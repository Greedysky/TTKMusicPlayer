#include "musicmarqueewidget.h"
#include <QtGui>

MusicMarqueeWidget::MusicMarqueeWidget(QWidget *parent)
    : QWidget(parent)
{
    m_offset = 0;
    m_myTimerId = 0;
}

void MusicMarqueeWidget::setText(const QString &newText)
{
    int length = fontMetrics().width(newText) >= width() ? 45 : 25;
    m_myText = newText.leftJustified(length, ' ');
    update();
    updateGeometry();
}

QSize MusicMarqueeWidget::sizeHint() const
{
    return fontMetrics().size(0, text());
}

void MusicMarqueeWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QFont f(font());
    f.setBold(false);
    painter.setFont(f);

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

void MusicMarqueeWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    m_myTimerId = startTimer(30);
}

void MusicMarqueeWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_myTimerId)
    {
        ++m_offset;
        if (m_offset >= fontMetrics().width(text()))
        {
            m_offset = 0;
        }
        scroll(-1, 0);
    }
    else
    {
        QWidget::timerEvent(event);
    }
}

void MusicMarqueeWidget::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    killTimer(m_myTimerId);
    m_myTimerId = 0;
}
