#ifndef TTKTIME_H
#define TTKTIME_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QDataStream>
#include "ttknumberdefine.h"
#include "ttkmoduleexport.h"

/*! @brief The class of the ttk time object.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKTime
{
    TTK_DECLARE_MODULE(TTKTime)
public:
    /*!
     * Object constructor.
     */
    TTKTime() noexcept;
    TTKTime(qint64 value) noexcept;
    TTKTime(int day, int hour, int min, int sec, int msec) noexcept;
    TTKTime(const TTKTime &other) noexcept;
    TTKTime(TTKTime &&other) noexcept;

    /*!
     * Check current time is null.
     */
    bool isNull() const noexcept;
    /*!
     * Check current time is valid.
     */
    bool isValid() const noexcept;

    /*!
     * Set current day.
     */
    inline void setDay(int day) noexcept { m_day = day; }
    /*!
     * Set current hour.
     */
    inline void setHour(int hour) noexcept { m_hour = hour; }
    /*!
     * Set current minute.
     */
    inline void setMinute(int min) noexcept { m_minute = min; }
    /*!
     * Set current second.
     */
    inline void setSecond(int sec) noexcept { m_second = sec; }
    /*!
     * Set current millionSecond.
     */
    inline void setMillionSecond(int msec) noexcept { m_msecond = msec; }

    /*!
     * Get current day.
     */
    inline int day() const noexcept { return m_day; }
    /*!
     * Get current hour.
     */
    inline int hour() const noexcept { return m_hour; }
    /*!
     * Get current second.
     */
    inline int minute() const noexcept { return m_minute; }
    /*!
     * Get current second.
     */
    inline int second() const noexcept { return m_second; }
    /*!
     * Get current millionSecond.
     */
    inline int millionSecond() const noexcept { return m_msecond; }

    /*!
     * Transform time from string by time format.
     */
    static TTKTime fromString(const QString &time, const QString &format) noexcept;
    /*!
     * Transform time from value to string time format.
     */
    static QString toString(qint64 time, const QString &format) noexcept;

    /*!
     * Transform time to string time format.
     */
    QString toString(const QString &format) const noexcept;
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

    /*!
     * Set current day and hour and min and sec and msec.
     */
    void fromValue(int day, int hour, int min, int sec, int msec = 0) noexcept;
    /*!
     * Transform from all ms time value.
     */
    void fromValue(qint64 value) noexcept;
    /*!
     * Transform to all ms time value.
     */
    qint64 toValue() const noexcept;

    /*!
     * Transform string format(mm:ss) to msec time.
     */
    static qint64 formatDuration(const QString &time) noexcept;
    /*!
     * Transform msec time to string format(mm:ss).
     */
    static QString formatDuration(qint64 time/*, bool greedy = true*/) noexcept;

    TTKTime& operator = (const TTKTime &other) noexcept;
    TTKTime& operator = (TTKTime &&other) noexcept;
    TTKTime& operator+= (const TTKTime &other) noexcept;
    TTKTime& operator+= (const int other) noexcept;
    TTKTime& operator-= (const TTKTime &other) noexcept;
    TTKTime& operator-= (const int other) noexcept;
    TTKTime& operator*= (const int other) noexcept;
    TTKTime& operator/= (const int other) noexcept;

    TTKTime  operator+ (const TTKTime &other) noexcept;
    TTKTime  operator+ (const int other) noexcept;
    TTKTime  operator- (const TTKTime &other) noexcept;
    TTKTime  operator- (const int other) noexcept;
    TTKTime  operator* (const int other) noexcept;
    TTKTime  operator/ (const int other) noexcept;

    bool operator== (const TTKTime &other) const noexcept;
    bool operator!= (const TTKTime &other) const noexcept;

    inline friend QDataStream& operator<<(QDataStream &stream, const TTKTime &other) noexcept
    {
        stream << other.day() << " " << other.hour() << " " << other.minute() << " "
               << other.second() << " " << other.millionSecond();
        return stream;
    }

    inline friend QDataStream& operator>>(QDataStream &stream, TTKTime &other) noexcept
    {
        int x[5];
        stream >> x[0] >> x[1] >> x[2] >> x[3] >> x[4];
        other.fromValue(x[0], x[1], x[2], x[3], x[4]);
        return stream;
    }

private:
    /*!
     * Init parameters;
     */
    void initialize() noexcept;
    /*!
     * Copy other time data to this obejct;
     */
    void copyToThis(const TTKTime &other) noexcept;

    int m_day, m_hour;
    int m_minute, m_second, m_msecond;

};


/*! @brief The class of the ttk date time object.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKDateTime
{
public:
    /*!
     * Get current timestamp(ms).
     */
    static qint64 currentTimestamp();
    /*!
     * Transform string time format to timestamp(ms) string.
     */
    static QString format(const QString &time, const QString &format);
    /*!
     * Transform timestamp(ms) to string time format.
     */
    static QString format(qint64 time, const QString &format);

};


/*! @brief The namespace of the random helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Init random value seed.
     */
    TTK_MODULE_EXPORT void initRandom();
    /*!
     * Get random value.
     */
    TTK_MODULE_EXPORT int random(int value = RAND_MAX);

}

#endif // TTKTIME_H
