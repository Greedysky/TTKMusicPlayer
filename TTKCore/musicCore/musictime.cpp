#include "musictime.h"

#include <QFileInfo>
#include <QDir>
#include <time.h>

MusicTime::MusicTime()
{
    m_defaultType = All_Msec;
    init();
}

MusicTime::MusicTime(qint64 value, Type type)
    : MusicTime()
{
    m_defaultType = type;
    fromTimeStamp(value, type == All_Sec ? 1 : 1000);
}

MusicTime::MusicTime(int day, int hour, int min, int sec, int msec)
    : MusicTime()
{
    m_defaultType = All_Msec;
    setHMSM(day, hour, min, sec, msec);
}

MusicTime::~MusicTime()
{

}

void MusicTime::init()
{
    m_day = 0;
    m_hour = 0;
    m_min = 0;
    m_sec = 0;
    m_msec = 0;
}

void MusicTime::setHMSM(int day, int hour, int min, int sec, int msec)
{
    init();

    int delta = 0;
    delta = msec >= 1000 ? msec / 1000 : 0;
    m_msec = msec % 1000;

    sec += delta;
    delta = sec >= 60 ? sec / 60 : 0;
    m_sec = sec % 60;

    min += delta;
    delta = min >= 60 ? min / 60 : 0;
    m_min = min % 60;

    hour += delta;
    m_day = hour >= 24 ? hour / 24 : 0;
    m_hour = hour % 24;

    m_day += day;
}

void MusicTime::fromTimeStamp(qint64 value, int delta)
{
    if(value < 0)
    {
        init();
        return;
    }

    m_day = value/3600/24/delta;
    value -= m_day*3600*24*delta;

    m_hour = value/3600/delta;
    value -= m_hour*3600*delta;

    m_min = value/60/delta;
    value -= m_min*60*delta;

    m_sec = value/delta;
    if(delta == 1000)
    {
        value -= (m_sec*delta);
        m_msec = value;
    }
}

bool MusicTime::isValid() const
{
    return !(m_hour == 0 && m_min == 0 && m_sec == 0 &&
             m_msec == 0);
}

MusicTime MusicTime::fromString(const QString &s, const QString &format)
{
    MusicTime musicTime;
    QTime t = QTime::fromString(s, format);
    musicTime.setHMSM(0, t.hour(), t.minute(), t.second(), t.msec());
    return musicTime;
}

QString MusicTime::toString(qint64 value, Type type, const QString &format)
{
    return MusicTime(value, type).toString(format);
}

QString MusicTime::toString(const QString &format)
{
    return QLocale::system().toString(
           QTime(m_hour, m_min, m_sec, m_msec), format);
}

qint64 MusicTime::getTimeStamp(Type type) const
{
    qint64 delta = (type == All_Sec) ? 1 : 1000;
           delta = (m_day*3600*24 + m_hour*3600 + m_min*60 +
                    m_sec)*delta;
    return (type == All_Sec) ? delta : (delta + m_msec);
}

void MusicTime::timeSRand()
{
    qsrand(time(nullptr));
}

QString MusicTime::msecTime2LabelJustified()
{
    return toString("mm:ss");
}

QString MusicTime::msecTime2LabelJustified(qint64 time)
{
    MusicTime t(time, MusicTime::All_Msec);
    return t.toString("mm:ss");
}

QString MusicTime::normalTime2LabelJustified()
{
    return toString("hh:mm:ss");
}

QString MusicTime::normalTime2LabelJustified(qint64 time)
{
    MusicTime t(time, MusicTime::All_Msec);
    return t.toString("hh:mm:ss");
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
    if(time < 60)
    {
        return QString::number(time) + QObject::tr("ss");
    }
    else if(60 <= time && time < 60*60)
    {
        return QString::number(time/60) + QObject::tr("mm");
    }
    else if(60*60 <= time && time < 60*60*24)
    {
        return QString::number(time/60/60) + QObject::tr("hh");
    }
    else
    {
        return QString::number(time/60/60/24) + QObject::tr("day");
    }
}

MusicTime& MusicTime::operator= (const MusicTime &other)
{
    m_defaultType = other.getType();
    m_day = other.getDay();
    m_hour = other.getHour();
    m_min = other.getMinute();
    m_sec = other.getSecond();
    m_msec = other.getMillionSecond();
    return *this;
}

MusicTime& MusicTime::operator+= (const MusicTime &other)
{
    qint64 t = getTimeStamp(All_Msec) + other.getTimeStamp(All_Msec);
    fromTimeStamp(t, m_defaultType == All_Sec ? 1 : 1000);
    return *this;
}

MusicTime& MusicTime::operator+= (const int other)
{
    qint64 t = getTimeStamp(All_Msec) + other;
    fromTimeStamp(t, m_defaultType == All_Sec ? 1 : 1000);
    return *this;
}

MusicTime& MusicTime::operator-= (const MusicTime &other)
{
    qint64 t = getTimeStamp(All_Msec) - other.getTimeStamp(All_Msec);
    fromTimeStamp(t, m_defaultType == All_Sec ? 1 : 1000);
    return *this;
}

MusicTime& MusicTime::operator-= (const int other)
{
    qint64 t = getTimeStamp(All_Msec) - other;
    fromTimeStamp(t, m_defaultType == All_Sec ? 1 : 1000);
    return *this;
}

MusicTime& MusicTime::operator*= (const int other)
{
    qint64 t = getTimeStamp(All_Msec) * other;
    fromTimeStamp(t, m_defaultType == All_Sec ? 1 : 1000);
    return *this;
}

MusicTime& MusicTime::operator/= (const int other)
{
    qint64 t = getTimeStamp(All_Msec) / other;
    fromTimeStamp(t, m_defaultType == All_Sec ? 1 : 1000);
    return *this;
}

MusicTime MusicTime::operator+ (const MusicTime &other)
{
    qint64 t = getTimeStamp(All_Msec) + other.getTimeStamp(All_Msec);
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator+ (const int other)
{
    qint64 t = getTimeStamp(All_Msec) + other;
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator- (const MusicTime &other)
{
    qint64 t = getTimeStamp(All_Msec) - other.getTimeStamp(All_Msec);
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator- (const int other)
{
    qint64 t = getTimeStamp(All_Msec) - other;
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator* (const int other)
{
    qint64 t = getTimeStamp(All_Msec) * other;
    return MusicTime(t, m_defaultType);
}

MusicTime MusicTime::operator/ (const int other)
{
    qint64 t = getTimeStamp(All_Msec) / other;
    return MusicTime(t, m_defaultType);
}

bool MusicTime::operator== (const MusicTime &other) const
{
    return getTimeStamp(All_Msec) == other.getTimeStamp(All_Msec);
}

bool MusicTime::operator!= (const MusicTime &other) const
{
    return getTimeStamp(All_Msec) != other.getTimeStamp(All_Msec);
}
