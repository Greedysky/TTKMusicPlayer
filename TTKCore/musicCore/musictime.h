#ifndef MUSICTIME_H
#define MUSICTIME_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDataStream>
#include "musicglobaldefine.h"
#include "musicnumberdefine.h"

/*! @brief The class of the music time object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicTime
{
public:
    enum Type
    {
        All_Msec,   ///*Current time entity is msec*/
        All_Sec     ///*Current time entity is sec*/
    };

    MusicTime();
    /*!
     * Object contsructor.
     */
    MusicTime(const MusicTime &other);
    /*!
     * Object contsructor.
     */
    MusicTime(qint64 value, Type type);
    /*!
     * Object contsructor by value and type.
     */
    MusicTime(int day, int hour, int min, int sec, int msec);
    /*!
     * Object contsructor by day and hour and min and sec and msec.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setHMSM(int day, int hour, int min, int sec, int msec = 0);
    /*!
     * Set current day and hour and min and sec and msec.
     */
    bool isNull() const;
    /*!
     * Check current time is null.
     */
    bool isValid() const;
    /*!
     * Check current time is valid.
     */

    inline void setType(Type type) { m_defaultType = type; }
    /*!
     * Set current time type, see Type.
     */
    inline Type getType() const { return m_defaultType; }
    /*!
     * Get current time type, see Type.
     */

    inline void setDay(int day) { m_day = day; }
    /*!
     * Set current day.
     */
    inline void setHour(int hour) { m_hour = hour; }
    /*!
     * Set current hour.
     */
    inline void setMinute(int min) { m_min = min; }
    /*!
     * Set current minute.
     */
    inline void setSecond(int sec) { m_sec = sec; }
    /*!
     * Set current second.
     */
    inline void setMillionSecond(int msec) { m_msec = msec; }
    /*!
     * Set current millionSecond.
     */
    inline int getDay() const { return m_day; }
    /*!
     * Get current day.
     */
    inline int getHour() const { return m_hour; }
    /*!
     * Get current hour.
     */
    inline int getMinute() const { return m_min; }
    /*!
     * Get current second.
     */
    inline int getSecond() const { return m_sec; }
    /*!
     * Get current second.
     */
    inline int getMillionSecond() const { return m_msec; }
    /*!
     * Get current millionSecond.
     */

    static MusicTime fromMSecsSinceEpoch(qint64 msecs);
    /*!
     * Transform ms time from utc since epoch.
     */
    static MusicTime fromString(const QString &s, const QString &format);
    /*!
     * Transform time from string by time format.
     */
    static QString toString(qint64 value, Type type, const QString &format);
    /*!
     * Transform time from value to string time format.
     */
    QString toString(const QString &format);
    /*!
     * Transform time to string time format.
     */
//    h	the hour without a leading zero (0 to 23 or 1 to 12 if AM/PM display)
//    hh	the hour with a leading zero (00 to 23 or 01 to 12 if AM/PM display)
//    H	the hour without a leading zero (0 to 23, even with AM/PM display)
//    HH	the hour with a leading zero (00 to 23, even with AM/PM display)
//    m	the minute without a leading zero (0 to 59)
//    mm	the minute with a leading zero (00 to 59)
//    s	the second without a leading zero (0 to 59)
//    ss	the second with a leading zero (00 to 59)
//    z	the milliseconds without leading zeroes (0 to 999)
//    zzz	the milliseconds with leading zeroes (000 to 999)
//    AP or A	use AM/PM display. A/AP will be replaced by either "AM" or "PM".
//    ap or a	use am/pm display. a/ap will be replaced by either "am" or "pm".
//    t	the timezone (for example "CEST")
    qint64 getTimeStamp(Type type) const;
    /*!
     * Get all time value by type.
     */

    //////////////////////////////////////////////
    static void timeSRand();
    /*!
     * Init random time seed.
     */
    //////////////////////////////////////////////
    QString msecTime2LabelJustified();
    /*!
     * Transform msec time to string format(mm:ss).
     */
    static QString msecTime2LabelJustified(qint64 time);
    /*!
     * Transform msec time to string format(mm:ss).
     */
    QString normalTime2LabelJustified();
    /*!
     * Transform msec time to string format(hh:mm:ss).
     */
    static QString normalTime2LabelJustified(qint64 time);
    /*!
     * Transform msec time to string format(hh:mm:ss).
     */
    QString normalTime2Label() const;
    /*!
     * Transform msec time to string format(/ ss / mm / hh / dd).
     */
    static QString normalTime2Label(qint64 time);
    /*!
     * Transform msec time to string format(/ ss / mm / hh / dd).
     */
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

    inline friend QDataStream& operator<<(QDataStream &stream, const MusicTime &other)
    {
        stream << other.getDay() << other.getHour() << other.getMinute()
               << other.getSecond() << other.getMillionSecond();
        return stream;
    }

    inline friend QDataStream& operator>>(QDataStream &stream, MusicTime &other)
    {
        int x[5];
        stream >> x[0] >> x[1] >> x[2] >> x[3] >> x[4];
        other.setHMSM(x[0], x[1], x[2], x[3], x[4]);
        return stream;
    }

protected:
    void init();
    /*!
     * Init parameters;
     */
    void copyToThis(const MusicTime &other);
    /*!
     * Copy other time data to this obejct;
     */
    void fromTimeStamp(qint64 value, int delta);
    /*!
     * Transform time value by different time type;
     */

    Type m_defaultType;
    int m_day, m_hour;
    int m_min, m_sec, m_msec;

};

#endif // MUSICTIME_H
