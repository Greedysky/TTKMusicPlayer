#include "musictextsliderwidget.h"

#include <QPainter>

MusicTextSliderWidget::MusicTextSliderWidget(QWidget *parent)
    : QWidget(parent),
      m_maxValue(1),
      m_currentValue(0)
{

}

void MusicTextSliderWidget::setMaxValue(int max)
{
    m_maxValue = max;
    update();
}

void MusicTextSliderWidget::setValue(int value)
{
    m_currentValue = value;
    update();
}

void MusicTextSliderWidget::setFrontText(const QString &text)
{
    m_frontText = text;
    update();
}

void MusicTextSliderWidget::setBackText(const QString &text)
{
    m_backText = text;
    update();
}

void MusicTextSliderWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.fillRect(0, 0, width(), 25, QColor(200, 200, 200, 70));

    if(m_currentValue != 0)
    {
        painter.fillRect(0, 0, m_currentValue * 1.0 / m_maxValue * width(), 20, QColor(100, 100, 100, 70));
    }

    painter.setPen(QColor(50, 50, 50));
    painter.drawText(QRect(10, 0, width() / 2, 20), Qt::AlignVCenter | Qt::AlignLeft, m_frontText);

    painter.setPen(QColor(50, 50, 50));
    painter.drawText(QRect(width() / 2, 0, width() / 2 - 10, 20), Qt::AlignVCenter | Qt::AlignRight, m_backText);
}
