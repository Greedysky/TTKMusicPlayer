#include "musicmarqueewidget.h"
#include "musicsettingmanager.h"
#include "musicwidgetutils.h"

MusicMarqueeWidget::MusicMarqueeWidget(QWidget *parent)
    : QWidget(parent),
      m_offset(0),
      m_timerId(0),
      m_effectOnResize(false)
{

}

void MusicMarqueeWidget::setText(const QString &newText)
{
    int width = 0;
    if(m_effectOnResize)
    {
        width = (G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN) / 10;
    }

    int length = TTK::Widget::fontTextWidth(font(), newText);
        length = (length >= this->width()) ? (45 + width) : (25 + width);
    m_text = newText.leftJustified(length, ' ');

    update();
    updateGeometry();
}

QSize MusicMarqueeWidget::sizeHint() const
{
    return fontMetrics().size(0, m_text);
}

void MusicMarqueeWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont f(font());
    f.setBold(false);
    painter.setFont(f);

    const int textWidth = TTK::Widget::fontTextWidth(font(), m_text);
    if(textWidth < 1)
    {
        return;
    }

    int x = -m_offset;
    while(x < width())
    {
        painter.drawText(x, 0, textWidth, height(), Qt::AlignLeft | Qt::AlignVCenter, m_text);
        x += textWidth;
    }
}

void MusicMarqueeWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    m_timerId = startTimer(30);
}

void MusicMarqueeWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerId)
    {
        ++m_offset;
        if(m_offset >= TTK::Widget::fontTextWidth(font(), m_text))
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
    killTimer(m_timerId);
    m_timerId = 0;
}

void MusicMarqueeWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    setText(text());
}
