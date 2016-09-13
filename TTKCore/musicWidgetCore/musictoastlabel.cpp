#include "musictoastlabel.h"
#include "musicnumberdefine.h"
#include "musicuiobject.h"

#include <QPainter>
#include <QPropertyAnimation>

MusicToastLabel::MusicToastLabel(QWidget *parent)
    : QLabel(parent)
{
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint );
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    m_font = font();
    connect(&m_timer, SIGNAL(timeout()), SLOT(closeAnimation()));
    m_timer.setInterval(MT_S2MS);
    m_timer.start();
}

MusicToastLabel::MusicToastLabel(const QString &text, QWidget *parent)
    : MusicToastLabel(parent)
{
    setText(text);
}

MusicToastLabel::~MusicToastLabel()
{
    m_timer.stop();
}

QString MusicToastLabel::getClassName()
{
    return staticMetaObject.className();
}

void MusicToastLabel::setFontMargin(int height, int width)
{
    m_margin.setX(height);
    m_margin.setY(width);
}

void MusicToastLabel::setTimerInterval(int msecond)
{
    m_timer.stop();
    m_timer.setInterval(msecond);
    m_timer.start();
}

int MusicToastLabel::getTimerInterval() const
{
    return m_timer.interval();
}

void MusicToastLabel::setFontSize(int size)
{
    m_font.setPointSize(size);
    setFont(m_font);
}

int MusicToastLabel::getFontSize() const
{
    return m_font.pointSize();
}

bool MusicToastLabel::bold() const
{
    return m_font.bold();
}

void MusicToastLabel::setBold(bool bold)
{
    m_font.setBold(bold);
    setFont(m_font);
}

void MusicToastLabel::setText(const QString &text)
{
    QFontMetrics metrics = QFontMetrics(m_font);
    setFixedSize(metrics.width(text) + m_margin.x(), metrics.height() + m_margin.y());
    QLabel::setText(text);
}

void MusicToastLabel::closeAnimation()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(MT_S2MS);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, SIGNAL(finished()), SLOT(close()));
}

void MusicToastLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 175));
    painter.drawRoundRect(rect(), 6, 6);

    painter.setPen(QColor(255, 255, 255));
    painter.drawText(rect(), Qt::AlignCenter, text());
    painter.end();
}
