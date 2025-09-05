#include "ttktoastlabel.h"
#include "ttkdesktopscreen.h"

#include <QPainter>
#include <QPropertyAnimation>

TTKToastLabel::TTKToastLabel(QWidget *parent)
    : QLabel(parent),
      m_font(font())
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setFontSize(15);
    setFontMargin(20, 20);

    connect(&m_timer, SIGNAL(timeout()), SLOT(closeAnimation()));
    m_timer.setInterval(TTK_DN_S2MS);
    m_timer.start();
}

TTKToastLabel::TTKToastLabel(const QString &text, QWidget *parent)
    : TTKToastLabel(parent)
{
    setText(text);
}

TTKToastLabel::~TTKToastLabel()
{
    m_timer.stop();
}

void TTKToastLabel::setFontMargin(int height, int width) noexcept
{
    m_margin.setX(height);
    m_margin.setY(width);
}

int TTKToastLabel::timerInterval() const noexcept
{
    return m_timer.interval();
}

void TTKToastLabel::setFontSize(int size)
{
    m_font.setPointSize(size);
    setFont(m_font);
}

void TTKToastLabel::popup(QWidget *parent)
{
    if(!parent)
    {
        const QRect &rect = TTKDesktopScreen::currentGeometry();
        move(rect.x() + (rect.width() - width()) / 2, rect.y() + rect.height() - 200);
    }
    else
    {
        const QPoint &point = parent->mapToGlobal(QPoint(0, 0));
        move(point.x() + (parent->width() - width()) / 2, point.y() + (parent->height() - height()) / 2);
    }
    show();
}

void TTKToastLabel::setText(const QString &text)
{
    const QFontMetrics ftm(m_font);
    setFixedSize(QtFontWidth(ftm, text) + m_margin.x(), ftm.height() + m_margin.y());
    QLabel::setText(text);
}

void TTKToastLabel::closeAnimation()
{
    m_timer.stop();

    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
    animation->setDuration(TTK_DN_S2MS);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();

    connect(animation, SIGNAL(finished()), SLOT(close()));
    connect(animation, SIGNAL(finished()), SIGNAL(finished()));
}

void TTKToastLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 175));
    painter.drawRoundedRect(rect(), 6, 6);

    painter.setPen(QColor(255, 255, 255));
    painter.drawText(rect(), Qt::AlignCenter, text());
}
