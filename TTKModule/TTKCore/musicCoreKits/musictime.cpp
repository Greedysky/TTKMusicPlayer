#include "musictime.h"
#include "qalgorithm/random.h"

#include <QDateTime>

MusicTime::MusicTime()
{
    m_defaultType = AllMsec;
    m_greedyMode = false;
    initialize();
}

MusicTime::MusicTime(const MusicTime &other)
{
    copyToThis(other);
}

MusicTime::MusicTime(qint64 value, Type type)
    : MusicTime()
{
    m_defaultType = type;
    fromTimeStamp(value, type == AllSec ? MT_S : MT_S2MS);
}

MusicTime::MusicTime(int day, int hour, int min, int sec, int msec)
    : MusicTime()
{
    m_defaultType = AllMsec;
    setHMSM(day, hour, min, sec, msec);
}

void MusicTime::setHMSM(int day, int hour, int min, int sec, int msec)
{
    initialize();

    int delta = 0;
    delta = msec >= MT_S2MS ? msec / MT_S2MS : 0;
    m_msec = msec % MT_S2MS;

    sec += delta;
    delta = sec >= MT_M2S ? sec / MT_M2S : 0;
    m_sec = sec % MT_M2S;

    min += delta;
    delta = min >= MT_H2M ? min / MT_H2M : 0;
    m_min = min % MT_H2M;

    hour += delta;
    m_day = hour >= MT_D2H ? hour / MT_D2H : 0;
    m_hour = hour % MT_D2H;

    m_day += day;
}

bool MusicTime::isNull() const
{
    return (m_hour == 0 && m_min == 0 && m_sec == 0 && m_msec == 0);
}

bool MusicTime::isValid() const
{
    return !isNull();
}

MusicTime MusicTime::fromString(const QString &s, const QString &format)
{
    MusicTime time;
    const QTime &t = QTime::fromString(s, format);
    time.setHMSM(0, t.hour(), t.minute(), t.second(), t.msec());
    return time;
}

QString MusicTime::toString(qint64 value, Type type, const QString &format)
{
    return MusicTime(value, type).toString(format);
}

QString MusicTime::toString(const QString &format) const
{
    return QTime(m_hour, m_min, m_sec, m_msec).toString(format);
}

qint64 MusicTime::timestamp(Type type) const
{
    qint64 delta = (type == AllSec) ? MT_S : MT_S2MS;
           delta = (m_day * MT_D2S + m_hour * MT_H2S + m_min * MT_M2S + m_sec) * delta;
    return (type == AllSec) ? delta : (delta + m_msec);
}

qint64 MusicTime::timestamp(bool ms)
{
    const qint64 t = QDateTime::currentMSecsSinceEpoch();
    return ms ? t : t / 1000;
}

void MusicTime::initRandom()
{
    QAlgorithm::initRandom();
}

int MusicTime::random(int value)
{
    return QAlgorithm::random(value);
}

qint64 MusicTime::labelJustified2MsecTime(const QString &time)
{
    MusicTime t = MusicTime::fromString(time, "mm:ss");
    return t.timestamp(MusicTime::AllMsec);
}

QString MusicTime::msecTime2LabelJustified()
{
    if(!m_greedyMode)
    {
        return toString("mm:ss");
    }
    else
    {
        const int min = m_day * MT_H2S + m_hour * MT_H + m_min;
        return QString::number(min).rightJustified(2, '0') + ":" + QString::number(m_sec).rightJustified(2, '0');
    }
}

QString MusicTime::msecTime2LabelJustified(qint64 time, bool greedy)
{
    MusicTime t(time, MusicTime::AllMsec);
    if(!greedy || time < MT_H2S * MT_S2MS)
    {
        return t.toString("mm:ss");
    }
    else
    {
        const int min = t.day() * MT_H2S + t.hour() * MT_H + t.minute();
        return QString::number(min).rightJustified(2, '0') + ":" + QString::number(t.second()).rightJustified(2, '0');
    }
}

QString MusicTime::normalTime2Label() const
{
    if(m_day == 0 && m_hour == 0 && m_min == 0)
    {
        return QString::number(m_sec) + QObject::tr("ss");
    }
    else if(m_day == 0 && m_hour == 0 && m_min != 0)
    {
        return QString::number(m_min) + QObject::tr("mm");
    }
    else if(m_day == 0 && m_hour != 0)
    {
        return QString::number(m_hour) + QObject::tr("hh");
    }
    else if(m_day != 0)
    {
        return QString::number(m_day) + QObject::tr("day");
    }
    else
    {
        return QString();
    }
}

QString MusicTime::normalTime2Label(qint64 time)
{
    if(time < MT_M2S)
    {
        return QString::number(time) + QObject::tr("ss");
    }
    else if(MT_M2S <= time && time < MT_H2S)
    {
        return QString::number(time / MT_M2S) + QObject::tr("mm");
    }
    else if(MT_H2S <= time && time < MT_D2S)
    {
        return QString::number(time / MT_H2S) + QObject::tr("hh");
    }
    else
    {
        return QString::number(time / MT_D2S) + QObject::tr("day");
    }
}

MusicTime& MusicTime::operator= (const MusicTime &other)
{
    copyToThis(other);
    return *this;
}

MusicTime& MusicTime::operator+= (const MusicTime &other)
{
    const qint64 t = timestamp(AllMsec) + other.timestamp(AllMsec);
    fromTimeStamp(t, m_defaultType == AllSec ? MT_S : MT_S2MS);
    return *this;
}

MusicTime& MusicTime::operator+= (const int other)
{
    const qint64 t = timestamp(AllMsec) + other;
    fromTimeStamp(t, m_defaultType == AllSec ? MT_S : MT_S2MS);
    return *this;
}

MusicTime& MusicTime::operator-= (const MusicTime &other)
{
    const qint64 t = timestamp(AllMsec) - other.timestamp(AllMsec);
    fromTimeStamp(t, m_defaultType == AllSec ? MT_S : MT_S2MS);
    return *this;
}

MusicTime& MusicTime::operator-= (const int other)
{
    const qint64 t = timestamp(AllMsec) - other;
    fromTimeStamp(t, m_defaultType == AllSec ? MT_S : MT_S2MS);
    return *this;
}

MusicTime& MusicTime::operator*= (const int other)
{
    const qint64 t = timestamp(AllMsec) * other;
    fromTimeStamp(t, m_defaultType == AllSec ? MT_S : MT_S2MS);
    return *this;
}

MusicTime& MusicTime::operator/= (const int other)
{
    const qint64 t = timestamp(AllMsec) / other;
    fromTimeStamp(t, m_defaultType == AllSec ? MT_S : MT_S2MS);
    return *this;
}

MusicTime MusicTime::operator+ (const MusicTime &other)
{
    const qint64 t = timestamp(AllMsec) + other.timestamp(AllMsec);
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator+ (const int other)
{
    const qint64 t = timestamp(AllMsec) + other;
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator- (const MusicTime &other)
{
    const qint64 t = timestamp(AllMsec) - other.timestamp(AllMsec);
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator- (const int other)
{
    const qint64 t = timestamp(AllMsec) - other;
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator* (const int other)
{
    const qint64 t = timestamp(AllMsec) * other;
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator/ (const int other)
{
    const qint64 t = timestamp(AllMsec) / other;
    return MusicTime(t, m_defaultType);
}

bool MusicTime::operator== (const MusicTime &other) const
{
    return timestamp(AllMsec) == other.timestamp(AllMsec);
}

bool MusicTime::operator!= (const MusicTime &other) const
{
    return timestamp(AllMsec) != other.timestamp(AllMsec);
}

void MusicTime::initialize()
{
    m_day = 0;
    m_hour = 0;
    m_min = 0;
    m_sec = 0;
    m_msec = 0;
}

void MusicTime::copyToThis(const MusicTime &other)
{
    m_greedyMode = other.m_greedyMode;
    m_defaultType = other.m_defaultType;
    m_day = other.m_day;
    m_hour = other.m_hour;
    m_min = other.m_min;
    m_sec = other.m_sec;
    m_msec = other.m_msec;
}

void MusicTime::fromTimeStamp(qint64 value, int delta)
{
    if(value < 0)
    {
        initialize();
        return;
    }

    m_day = value / MT_D2S / delta;
    value -= m_day * MT_D2S * delta;

    m_hour = value / MT_H2S / delta;
    value -= m_hour * MT_H2S * delta;

    m_min = value / MT_M2S / delta;
    value -= m_min * MT_M2S * delta;

    m_sec = value / delta;
    if(delta == MT_S2MS)
    {
        value -= (m_sec * delta);
        m_msec = value;
    }
}
