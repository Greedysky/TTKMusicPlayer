#include "musicroundanimationlabel.h"

#include <QPainter>

MusicRoundAnimationLabel::MusicRoundAnimationLabel(QWidget *parent)
    : QWidget(parent),
      m_rotateAngle(0)
{
    m_timer.setInterval(50 * TTK_DN_MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(update()));
}

MusicRoundAnimationLabel::~MusicRoundAnimationLabel()
{
    stop();
}

void MusicRoundAnimationLabel::setPixmap(const QPixmap &pix) noexcept
{
    m_pixmap = pix;
}

void MusicRoundAnimationLabel::setInterval(int msec)
{
    m_timer.setInterval(msec);
}

void MusicRoundAnimationLabel::start()
{
    m_timer.start();
}

void MusicRoundAnimationLabel::stop()
{
    m_timer.stop();
}

void MusicRoundAnimationLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    const QSize &self = size();
    const QRect rotatedRect(-self.width() / 2, -self.height() / 2, self.width(), self.height());
    const int cx = self.width() / 2;
    const int cy = self.height() / 2;

    if(++m_rotateAngle >= TTK_AN_360)
    {
        m_rotateAngle -= TTK_AN_360;
    }

    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.translate(cx, cy);
    painter.rotate(m_rotateAngle);
    painter.drawPixmap(rotatedRect, m_pixmap);
    painter.restore();
}
