#include "musictoastlabel.h"
#include "musicnumberdefine.h"

#include <QPropertyAnimation>

MusicToastLabel::MusicToastLabel(QWidget *parent)
    : QLabel(parent)
{
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint );
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet("background:#F9D982; color:white;");

    m_font = font();
    connect(&m_timer, SIGNAL(timeout()), SLOT(closeAnimation()));
    m_timer.setInterval(2*MT_S2MS);
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
    setFixedSize(metrics.width(text), metrics.height());
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
