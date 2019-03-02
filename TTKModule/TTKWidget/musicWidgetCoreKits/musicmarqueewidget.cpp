#include "musicmarqueewidget.h"
#include "musicsettingmanager.h"

#include <QPainter>

MusicMarqueeWidget::MusicMarqueeWidget(QWidget *parent)
    : QWidget(parent)
{
    m_offset = 0;
    m_myTimerId = 0;
    m_effectOnResize = false;
}

void MusicMarqueeWidget::setText(const QString &newText)
{
    int w = 0;
    if(m_effectOnResize)
    {
        w = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
        w = (w - WINDOW_WIDTH_MIN)/10;
    }

    int length = fontMetrics().width( newText );
        length = (length >= width()) ? (45 + w) : (25 + w);
    m_myText = newText.leftJustified(length, ' ');

    update();
    updateGeometry();
}

QSize MusicMarqueeWidget::sizeHint() const
{
    return fontMetrics().size(0, m_myText);
}

void MusicMarqueeWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QFont f(font());
    f.setBold(false);
    painter.setFont(f);

    const int textWidth = fontMetrics().width(m_myText);
    if(textWidth < 1)
    {
        return;
    }

    int x = -m_offset;
    while(x < width())
    {
        painter.drawText(x, 0, textWidth, height(), Qt::AlignLeft | Qt::AlignVCenter, m_myText);
        x += textWidth;
    }
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
        if(m_offset >= fontMetrics().width(m_myText))
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

void MusicMarqueeWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    setText( text() );
}
