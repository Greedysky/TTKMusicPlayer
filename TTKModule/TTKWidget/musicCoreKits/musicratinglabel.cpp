#include "musicratinglabel.h"

#include <QPainter>

MusicRatingLabel::MusicRatingLabel(QWidget *parent)
    : MusicRatingLabel(0, parent)
{

}

MusicRatingLabel::MusicRatingLabel(int value, QWidget *parent)
    : QLabel(parent),
      m_value(value)
{
    setFixedSize(26 * 5, 22);
}

void MusicRatingLabel::setValue(int value) noexcept
{
    m_value = value;
}

void MusicRatingLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    for(int i = 0; i < 5; ++i)
    {
        const bool active = m_value - i - 1 >= 0;
        painter.drawPixmap(i * 26, 0, QPixmap(active ? ":/tiny/lb_star" : ":/tiny/lb_unstar"));
    }
}
