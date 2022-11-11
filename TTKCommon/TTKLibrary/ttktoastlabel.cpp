#include "ttktoastlabel.h"
#include "ttkdesktopwrapper.h"
#include "ttknumberdefine.h"

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
    m_timer.setInterval(MT_S2MS);
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

void TTKToastLabel::setFontMargin(int height, int width)
{
    m_margin.setX(height);
    m_margin.setY(width);
}

int TTKToastLabel::timerInterval() const
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
        const QSize &windowSize = TTKDesktopWrapper::screenGeometry().size();
        move((windowSize.width() - width()) / 2, windowSize.height() - 200);
    }
    else
    {
        const QPoint &globalPoint = parent->mapToGlobal(QPoint(0, 0));
        move(globalPoint.x() + (parent->width() - width()) / 2, globalPoint.y() + (parent->height() - height()) / 2);
    }
    show();
}

void TTKToastLabel::setText(const QString &text)
{
    QFontMetrics ft(m_font);
    setFixedSize(QtFontWidth(ft, text) + m_margin.x(), ft.height() + m_margin.y());
    QLabel::setText(text);
}

void TTKToastLabel::closeAnimation()
{
    m_timer.stop();

    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
    animation->setDuration(MT_S2MS);
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
    QtDrawRoundedRect(&painter, rect(), 6, 6);

    painter.setPen(QColor(255, 255, 255));
    painter.drawText(rect(), Qt::AlignCenter, text());
}
