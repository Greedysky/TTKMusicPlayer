#ifndef MUSICTIME_H
#define MUSICTIME_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDataStream>
#include "musiclibexportglobal.h"

class MUSIC_CORE_EXPORT MusicTime : public QObject
{
    Q_OBJECT
public:
    enum Type { All_Msec, All_Sec};

    explicit MusicTime(QObject *parent = 0);
    MusicTime(qint64 value, Type type, QObject *parent = 0);
    MusicTime(int day, int hour, int min, int sec, int msec = 0,
              QObject *parent = 0);
    ~MusicTime();

    void setHMSM(int day, int hour, int min, int sec, int msec = 0);
    bool isValid() const;

    void setType(Type type) {m_defaultType = type;}
    Type getType() const {return m_defaultType;}

    void setDay(int day) {m_day = day;}
    void setHour(int hour) {m_hour = hour;}
    void setMinute(int min) {m_min = min;}
    void setSecond(int sec) {m_sec = sec;}
    void setMillionSecond(int msec) {m_msec = msec;}

    int getDay() const {return m_day;}
    int getHour() const {return m_hour;}
    int getMinute() const {return m_min;}
    int getSecond() const {return m_sec;}
    int getMillionSecond() const {return m_msec;}

    qint64 getTimeStamp(Type type) const;

    QString msecTime2LabelJustified() const;
    static QString msecTime2LabelJustified(qint64 time);
    // xx:xx:xx
    QString normalTime2LabelJustified() const;
    static QString normalTime2LabelJustified(qint64 time);
    //ss / mm / hh / dd
    QString normalTime2Label() const;
    static QString normalTime2Label(qint64 time);

    //////////////////////////////////////////////
    static QString fileSzie2Label(qint64 size);
    // xx KB
    static qreal fileSzieByte2KByte(qint64 size);
    // xx MB
    static qreal fileSzieByte2MByte(qint64 size);
    //////////////////////////////////////////////

    MusicTime& operator= (const MusicTime &other);
    MusicTime& operator+= (const MusicTime &other);
    MusicTime& operator+= (const int other);
    MusicTime& operator-= (const MusicTime &other);
    MusicTime& operator-= (const int other);
    MusicTime& operator*= (const int other);
    MusicTime& operator/= (const int other);

    MusicTime  operator+ (const MusicTime &other);
    MusicTime  operator+ (const int other);
    MusicTime  operator- (const MusicTime &other);
    MusicTime  operator- (const int other);
    MusicTime  operator* (const int other);
    MusicTime  operator/ (const int other);

    bool operator== (const MusicTime &other) const;
    bool operator!= (const MusicTime &other) const;

    friend QDataStream& operator<<(QDataStream& stream, const MusicTime &other)
    {
        stream<<other.getDay()<<other.getHour()<<other.getMinute()
              <<other.getSecond()<<other.getMillionSecond();
        return stream;
    }

    friend QDataStream& operator>>(QDataStream& stream, MusicTime &other)
    {
        int x[5];
        stream>>x[0]>>x[1]>>x[2]>>x[3]>>x[4];
        other.setHMSM(x[0], x[1], x[2], x[3], x[4]);

        return stream;
    }

protected:
    void init();
    void fromTimeStamp(qint64 value, int delta);

    Type m_defaultType;
    int m_day, m_hour;
    int m_min, m_sec, m_msec;

};

#endif // MUSICTIME_H
