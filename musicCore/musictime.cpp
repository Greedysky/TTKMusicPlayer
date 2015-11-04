#include "musictime.h"

MusicTime::MusicTime(QObject *parent)
    : QObject(parent)
{
    m_defaultType = All_Msec;
    init();
}

MusicTime::MusicTime(qint64 value, Type type, QObject *parent)
    : MusicTime(parent)
{
    m_defaultType = type;
    fromTimeStamp(value, type == All_Sec ? 1 : 1000);
}

MusicTime::MusicTime(int day, int hour, int min, int sec, int msec,
      QObject *parent) : MusicTime(parent)
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

qint64 MusicTime::getTimeStamp(Type type) const
{
    qint64 delta = (type == All_Sec) ? 1 : 1000;
           delta = (m_day*3600*24 + m_hour*3600 + m_min*60 +
                    m_sec)*delta;
    return (type == All_Sec) ? delta : (delta + m_msec);
}

QString MusicTime::msecTime2LabelJustified() const
{
    return QString("%1:%2")
          .arg(QString::number(m_min).rightJustified(2, '0'))
          .arg(QString::number(m_sec).rightJustified(2, '0'));
}

QString MusicTime::msecTime2LabelJustified(qint64 time)
{
    int minute = time / (60 * 1000);
    int second = time % (60 * 1000);
    return QString("%1:%2")
          .arg(QString::number(minute).rightJustified(2, '0'))
          .arg(QString::number(second / 1000).rightJustified(2, '0'));
}

QString MusicTime::normalTime2LabelJustified() const
{
    return QString("%1:%2:%3")
          .arg(QString::number(m_hour).rightJustified(2, '0'))
          .arg(QString::number(m_min).rightJustified(2, '0'))
          .arg(QString::number(m_sec).rightJustified(2, '0'));
}

QString MusicTime::normalTime2LabelJustified(qint64 time)
{
    int hour = time / 3600;
    int min = (time - hour*3600) / 60;
    int sec = (time - min*60);
    return QString("%1:%2:%3")
          .arg(QString::number(hour).rightJustified(2, '0'))
          .arg(QString::number(min).rightJustified(2, '0'))
          .arg(QString::number(sec).rightJustified(2, '0'));
}

QString MusicTime::normalTime2Label() const
{
    if(m_day == 0 && m_hour == 0 && m_min == 0)
    {
        return QString::number(m_sec) + tr("ss");
    }
    else if(m_day == 0 && m_hour == 0 && m_min != 0)
    {
        return QString::number(m_min) + tr("mm");
    }
    else if(m_day == 0 && m_hour != 0)
    {
        return QString::number(m_hour) + tr("hh");
    }
    else if(m_day != 0)
    {
        return QString::number(m_day) + tr("day");
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
        return QString::number(time) + tr("ss");
    }
    else if(60 <= time && time < 60*60)
    {
        return QString::number(time/60) + tr("mm");
    }
    else if(60*60 <= time && time < 60*60*24)
    {
        return QString::number(time/60/60) + tr("hh");
    }
    else
    {
        return QString::number(time/60/60/24) + tr("day");
    }
}

QString MusicTime::fileSzie2Label(qint64 size)
{
    if( size < 1024)
    {
        return QString("%1 Byte").arg(size);
    }
    else if( 1024 <= size && size < 1024*1024)
    {
        return QString("%1 KByte").arg((qint64)(size*1.0/1024*100)/100.0);
    }
    else if( 1024*1024 <= size && size < 1024*1024*1024)
    {
        return QString("%1 MByte").arg((qint64)(size*1.0/1024/1024*100)/100.0);
    }
    else
    {
        return QString();
    }
}

qreal MusicTime::fileSzieByte2KByte(qint64 size)
{
    return size / 1024.0;
}

qreal MusicTime::fileSzieByte2MByte(qint64 size)
{
    return fileSzieByte2KByte(size) / 1024.0;
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
