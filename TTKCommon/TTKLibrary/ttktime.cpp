#include "ttktime.h"

#if TTK_QT_VERSION_CHECK(5,10,0)
#  include <QRandomGenerator>
#endif

namespace QAlgorithm
{
void initRandom()
{
#if !TTK_QT_VERSION_CHECK(5,10,0)
    qsrand(QDateTime::currentMSecsSinceEpoch());
#endif
}

int random(int value)
{
#if TTK_QT_VERSION_CHECK(5,10,0)
    return QRandomGenerator::global()->bounded(value);
#else
    return qrand() % value;
#endif
}
}


TTKTime::TTKTime()
    : m_greedyMode(false),
      m_defaultType(Entity::Millisecond)
{
    initialize();
}

TTKTime::TTKTime(const TTKTime &other)
{
    copyToThis(other);
}

TTKTime::TTKTime(qint64 value, Entity type)
    : TTKTime()
{
    m_defaultType = type;
    fromTimeStamp(value, type == Entity::Second ? MT_S : MT_S2MS);
}

TTKTime::TTKTime(int day, int hour, int min, int sec, int msec)
    : TTKTime()
{
    m_defaultType = Entity::Millisecond;
    setHMSM(day, hour, min, sec, msec);
}

void TTKTime::setHMSM(int day, int hour, int min, int sec, int msec)
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

bool TTKTime::isNull() const
{
    return (m_hour == 0 && m_min == 0 && m_sec == 0 && m_msec == 0);
}

bool TTKTime::isValid() const
{
    return !isNull();
}

TTKTime TTKTime::fromString(const QString &s, const QString &format)
{
    TTKTime time;
    const QTime &t = QTime::fromString(s, format);
    time.setHMSM(0, t.hour(), t.minute(), t.second(), t.msec());
    return time;
}

QString TTKTime::toString(qint64 value, Entity type, const QString &format)
{
    return TTKTime(value, type).toString(format);
}

QString TTKTime::toString(const QString &format) const
{
    return QTime(m_hour, m_min, m_sec, m_msec).toString(format);
}

qint64 TTKTime::timestamp(Entity type) const
{
    qint64 delta = (type == Entity::Second) ? MT_S : MT_S2MS;
           delta = (m_day * MT_D2S + m_hour * MT_H2S + m_min * MT_M2S + m_sec) * delta;
    return (type == Entity::Second) ? delta : (delta + m_msec);
}

qint64 TTKTime::timestamp(bool ms)
{
    const qint64 t = QDateTime::currentMSecsSinceEpoch();
    return ms ? t : t / 1000;
}

void TTKTime::initRandom()
{
    QAlgorithm::initRandom();
}

int TTKTime::random(int value)
{
    return QAlgorithm::random(value);
}

qint64 TTKTime::labelJustified2MsecTime(const QString &time)
{
    TTKTime t = TTKTime::fromString(time, "mm:ss");
    return t.timestamp(Entity::Millisecond);
}

QString TTKTime::msecTime2LabelJustified()
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

QString TTKTime::msecTime2LabelJustified(qint64 time, bool greedy)
{
    TTKTime t(time, Entity::Millisecond);
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

QString TTKTime::normalTime2Label() const
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

QString TTKTime::normalTime2Label(qint64 time)
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

TTKTime& TTKTime::operator= (const TTKTime &other)
{
    copyToThis(other);
    return *this;
}

TTKTime& TTKTime::operator+= (const TTKTime &other)
{
    const qint64 t = timestamp(Entity::Millisecond) + other.timestamp(Entity::Millisecond);
    fromTimeStamp(t, m_defaultType == Entity::Second ? MT_S : MT_S2MS);
    return *this;
}

TTKTime& TTKTime::operator+= (const int other)
{
    const qint64 t = timestamp(Entity::Millisecond) + other;
    fromTimeStamp(t, m_defaultType == Entity::Second ? MT_S : MT_S2MS);
    return *this;
}

TTKTime& TTKTime::operator-= (const TTKTime &other)
{
    const qint64 t = timestamp(Entity::Millisecond) - other.timestamp(Entity::Millisecond);
    fromTimeStamp(t, m_defaultType == Entity::Second ? MT_S : MT_S2MS);
    return *this;
}

TTKTime& TTKTime::operator-= (const int other)
{
    const qint64 t = timestamp(Entity::Millisecond) - other;
    fromTimeStamp(t, m_defaultType == Entity::Second ? MT_S : MT_S2MS);
    return *this;
}

TTKTime& TTKTime::operator*= (const int other)
{
    const qint64 t = timestamp(Entity::Millisecond) * other;
    fromTimeStamp(t, m_defaultType == Entity::Second ? MT_S : MT_S2MS);
    return *this;
}

TTKTime& TTKTime::operator/= (const int other)
{
    const qint64 t = timestamp(Entity::Millisecond) / other;
    fromTimeStamp(t, m_defaultType == Entity::Second ? MT_S : MT_S2MS);
    return *this;
}

TTKTime TTKTime::operator+ (const TTKTime &other)
{
    const qint64 t = timestamp(Entity::Millisecond) + other.timestamp(Entity::Millisecond);
    return TTKTime(t, m_defaultType);
}

TTKTime TTKTime::operator+ (const int other)
{
    const qint64 t = timestamp(Entity::Millisecond) + other;
    return TTKTime(t, m_defaultType);
}

TTKTime TTKTime::operator- (const TTKTime &other)
{
    const qint64 t = timestamp(Entity::Millisecond) - other.timestamp(Entity::Millisecond);
    return TTKTime(t, m_defaultType);
}

TTKTime TTKTime::operator- (const int other)
{
    const qint64 t = timestamp(Entity::Millisecond) - other;
    return TTKTime(t, m_defaultType);
}

TTKTime TTKTime::operator* (const int other)
{
    const qint64 t = timestamp(Entity::Millisecond) * other;
    return TTKTime(t, m_defaultType);
}

TTKTime TTKTime::operator/ (const int other)
{
    const qint64 t = timestamp(Entity::Millisecond) / other;
    return TTKTime(t, m_defaultType);
}

bool TTKTime::operator== (const TTKTime &other) const
{
    return timestamp(Entity::Millisecond) == other.timestamp(Entity::Millisecond);
}

bool TTKTime::operator!= (const TTKTime &other) const
{
    return timestamp(Entity::Millisecond) != other.timestamp(Entity::Millisecond);
}

void TTKTime::initialize()
{
    m_day = 0;
    m_hour = 0;
    m_min = 0;
    m_sec = 0;
    m_msec = 0;
}

void TTKTime::copyToThis(const TTKTime &other)
{
    m_greedyMode = other.m_greedyMode;
    m_defaultType = other.m_defaultType;
    m_day = other.m_day;
    m_hour = other.m_hour;
    m_min = other.m_min;
    m_sec = other.m_sec;
    m_msec = other.m_msec;
}

void TTKTime::fromTimeStamp(qint64 value, int delta)
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
