#ifndef TTKLOGGER_H
#define TTKLOGGER_H

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

#include <QDebug>
#include <QDateTime>

#if QT_VERSION < QT_VERSION_CHECK(5,4,0)
#  define TTK_LOG_OUTPUT qDebug()
#else
#  define TTK_LOG_OUTPUT qDebug().noquote()
#endif

#define __TTK_DATE__ QDate::currentDate().toString("yyyy-MM-dd")
#define __TTK_TIME__ QTime::currentTime().toString("hh:mm:ss:zzz")

// log stream native macro
#define TTK_LOG_STREAM(msg) TTK_LOG_OUTPUT << msg
// log stream base macro
#define TTK_BASE_STREAM(level, msg) TTK_LOG_OUTPUT << QString("[%1 %2][%3][%4(%5)] ").arg(__TTK_DATE__, __TTK_TIME__, level, __FILE__, QString::number(__LINE__)) << msg
// log stream once base macro
#define TTK_ONCE_STREAM(level, msg) \
  static bool __hit__ = false; \
  if(!__hit__) \
  { \
    __hit__ = true; \
    TTK_BASE_STREAM(level, msg); \
  }
// log stream count base macro
#define TTK_COUNT_STREAM(count, level, msg) \
  static  __last__ = 1; \
  if(count > 0 && ++__last__ > count) \
  { \
    __last__ = 1; \
    TTK_BASE_STREAM(level, msg); \
  }
// log stream period base macro
#define TTK_PERIOD_STREAM(period, level, msg) \
  static qint64 __last__ = 0; \
  const qint64 __now__ = QDateTime::currentMSecsSinceEpoch(); \
  if(__last__ + period * 1000 <= __now__ || __now__ < __last__) \
  { \
    __last__ = __now__; \
    TTK_BASE_STREAM(level, msg); \
  }

#define TTK_TRACE_STREAM(msg) { TTK_BASE_STREAM("I", msg); }
#define TTK_DEBUG_STREAM(msg) { TTK_BASE_STREAM("D", msg); }
#define TTK_INFO_STREAM(msg)  { TTK_BASE_STREAM("W", msg); }
#define TTK_WARN_STREAM(msg)  { TTK_BASE_STREAM("T", msg); }
#define TTK_ERROR_STREAM(msg) { TTK_BASE_STREAM("E", msg); }
#define TTK_FATAL_STREAM(msg) { TTK_BASE_STREAM("F", msg); }

#define TTK_TRACE_STREAM_ONCE(msg) { TTK_ONCE_STREAM("I", msg) }
#define TTK_DEBUG_STREAM_ONCE(msg) { TTK_ONCE_STREAM("D", msg) }
#define TTK_INFO_STREAM_ONCE(msg)  { TTK_ONCE_STREAM("W", msg) }
#define TTK_WARN_STREAM_ONCE(msg)  { TTK_ONCE_STREAM("T", msg) }
#define TTK_ERROR_STREAM_ONCE(msg) { TTK_ONCE_STREAM("E", msg) }
#define TTK_FATAL_STREAM_ONCE(msg) { TTK_ONCE_STREAM("F", msg) }

#define TTK_TRACE_STREAM_COUNT(count, msg) { TTK_COUNT_STREAM(count, "I", msg) }
#define TTK_DEBUG_STREAM_COUNT(count, msg) { TTK_COUNT_STREAM(count, "D", msg) }
#define TTK_INFO_STREAM_COUNT(count, msg)  { TTK_COUNT_STREAM(count, "W", msg) }
#define TTK_WARN_STREAM_COUNT(count, msg)  { TTK_COUNT_STREAM(count, "T", msg) }
#define TTK_ERROR_STREAM_COUNT(count, msg) { TTK_COUNT_STREAM(count, "E", msg) }
#define TTK_FATAL_STREAM_COUNT(count, msg) { TTK_COUNT_STREAM(count, "F", msg) }

#define TTK_TRACE_STREAM_PERIOD(period, msg) { TTK_PERIOD_STREAM(period, "I", msg) }
#define TTK_DEBUG_STREAM_PERIOD(period, msg) { TTK_PERIOD_STREAM(period, "D", msg) }
#define TTK_INFO_STREAM_PERIOD(period, msg)  { TTK_PERIOD_STREAM(period, "W", msg) }
#define TTK_WARN_STREAM_PERIOD(period, msg)  { TTK_PERIOD_STREAM(period, "T", msg) }
#define TTK_ERROR_STREAM_PERIOD(period, msg) { TTK_PERIOD_STREAM(period, "E", msg) }
#define TTK_FATAL_STREAM_PERIOD(period, msg) { TTK_PERIOD_STREAM(period, "F", msg) }

#endif // TTKLOGGER_H