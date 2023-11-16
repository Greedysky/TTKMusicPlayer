#include "ttktime.h"

#if TTK_QT_VERSION_CHECK(5,10,0)
#  include <QRandomGenerator>
#endif

void TTK::initRandom()
{
#if !TTK_QT_VERSION_CHECK(5,10,0)
    qsrand(QDateTime::currentMSecsSinceEpoch());
#endif
}

int TTK::random(int value)
{
#if TTK_QT_VERSION_CHECK(5,10,0)
    return QRandomGenerator::global()->bounded(value);
#else
    return qrand() % value;
#endif
}



TTKTime::TTKTime()
{
    initialize();
}

TTKTime::TTKTime(qint64 value)
    : TTKTime()
{
    initialize();
    fromValue(value);
}

TTKTime::TTKTime(int day, int hour, int min, int sec, int msec)
    : TTKTime()
{
    initialize();
    fromValue(day, hour, min, sec, msec);
}

TTKTime::TTKTime(const TTKTime &other)
{
    copyToThis(other);
}

TTKTime::TTKTime(TTKTime &&other)
{
    copyToThis(other);
}

bool TTKTime::isNull() const
{
    return m_hour == 0 && m_minute == 0 && m_second == 0 && m_msecond == 0;
}

bool TTKTime::isValid() const
{
    return !isNull();
}

TTKTime TTKTime::fromString(const QString &time, const QString &format)
{
    TTKTime t;
    const QTime &qtime = QTime::fromString(time, format);
    t.fromValue(0, qtime.hour(), qtime.minute(), qtime.second(), qtime.msec());
    return t;
}

QString TTKTime::toString(qint64 time, const QString &format)
{
    return TTKTime(time).toString(format);
}

QString TTKTime::toString(const QString &format) const
{
    return QTime(m_hour, m_minute, m_second, m_msecond).toString(format);
}

void TTKTime::fromValue(int day, int hour, int min, int sec, int msec)
{
    if(day < 0 || hour < 0 || hour > 24 || min < 0 || min > 60 || sec < 0 || sec > 60 || msec < 0 || msec > 1000)
    {
        return;
    }

    m_day = day;
    m_hour = hour;
    m_minute = min;
    m_second = sec;
    m_msecond = msec;
}

void TTKTime::fromValue(qint64 value)
{
    if(value < 0)
    {
        return;
    }

    m_msecond = value % TTK_DN_S2MS;
    value /= TTK_DN_S2MS;

    m_day = value / TTK_DN_D2S;
    value %= TTK_DN_D2S;

    m_hour = value / TTK_DN_H2S;
    value %= TTK_DN_H2S;

    m_minute = value / TTK_DN_M2S;
    value %= TTK_DN_M2S;

    m_second = value;
}

qint64 TTKTime::toValue() const
{
    return (m_day * TTK_DN_D2S + m_hour * TTK_DN_H2S + m_minute * TTK_DN_M2S + m_second) * TTK_DN_S2MS + m_msecond;
}

qint64 TTKTime::formatDuration(const QString &time)
{
    return TTKTime::fromString(time, "mm:ss").toValue();
}

QString TTKTime::formatDuration(qint64 time/*, bool greedy*/)
{
    const TTKTime t(time);
    if(/*!greedy || */time < TTK_DN_H2S * TTK_DN_S2MS)
    {
        return t.toString("mm:ss");
    }
    else
    {
        const int min = t.day() * TTK_DN_H2S + t.hour() * TTK_DN_H + t.minute();
        return QString::number(min).rightJustified(2, '0') + ":" + QString::number(t.second()).rightJustified(2, '0');
    }
}

TTKTime& TTKTime::operator= (const TTKTime &other)
{
    copyToThis(other);
    return *this;
}

TTKTime& TTKTime::operator= (TTKTime &&other)
{
    copyToThis(other);
    return *this;
}

TTKTime& TTKTime::operator+= (const TTKTime &other)
{
    fromValue(toValue() + other.toValue());
    return *this;
}

TTKTime& TTKTime::operator+= (const int other)
{
    fromValue(toValue() + other);
    return *this;
}

TTKTime& TTKTime::operator-= (const TTKTime &other)
{
    fromValue(toValue() - other.toValue());
    return *this;
}

TTKTime& TTKTime::operator-= (const int other)
{
    fromValue(toValue() - other);
    return *this;
}

TTKTime& TTKTime::operator*= (const int other)
{
    fromValue(toValue() * other);
    return *this;
}

TTKTime& TTKTime::operator/= (const int other)
{
    fromValue(toValue() / other);
    return *this;
}

TTKTime TTKTime::operator+ (const TTKTime &other)
{
    return TTKTime(toValue() + other.toValue());
}

TTKTime TTKTime::operator+ (const int other)
{
    return TTKTime(toValue() + other);
}

TTKTime TTKTime::operator- (const TTKTime &other)
{
    return TTKTime(toValue() - other.toValue());
}

TTKTime TTKTime::operator- (const int other)
{
    return TTKTime(toValue() - other);
}

TTKTime TTKTime::operator* (const int other)
{
    return TTKTime(toValue() * other);
}

TTKTime TTKTime::operator/ (const int other)
{
    return TTKTime(toValue() / other);
}

bool TTKTime::operator== (const TTKTime &other) const
{
    return toValue() == other.toValue();
}

bool TTKTime::operator!= (const TTKTime &other) const
{
    return toValue() != other.toValue();
}

void TTKTime::initialize()
{
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
    m_second = 0;
    m_msecond = 0;
}

void TTKTime::copyToThis(const TTKTime &other)
{
    m_day = other.m_day;
    m_hour = other.m_hour;
    m_minute = other.m_minute;
    m_second = other.m_second;
    m_msecond = other.m_msecond;
}


qint64 TTKDateTime::currentTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch();
}

QString TTKDateTime::format(const QString &time, const QString &format)
{
    return QString::number(QDateTime::fromString(time, format).toMSecsSinceEpoch());
}

QString TTKDateTime::format(qint64 time, const QString &format)
{
    return QDateTime::fromMSecsSinceEpoch(time).toString(format);
}
