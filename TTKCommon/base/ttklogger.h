#ifndef TTKLOGGER_H
#define TTKLOGGER_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
#include "ttkglobal.h"

#if QT_VERSION < QT_VERSION_CHECK(5,4,0)
#  define __TTK_LOG_OUTPUT__ qDebug()
#else
#  define __TTK_LOG_OUTPUT__ qDebug().noquote()
#endif

#define __TTK_DATE__ QDate::currentDate().toString(TTK_DATE_FORMAT)
#define __TTK_TIME__ QTime::currentTime().toString(TTK_TIMEZ_FORMAT)

// log stream base macro
#define __TTK_BASE_STREAM__(level, msg) __TTK_LOG_OUTPUT__ << QString("[%1 %2][%3][%4(%5)] ").arg(__TTK_DATE__, __TTK_TIME__, level, __FILE__, QString::number(__LINE__)) << msg
// log stream once base macro
#define __TTK_ONCE_STREAM__(level, msg) \
  static bool __hit__ = false; \
  if(!__hit__) \
  { \
    __hit__ = true; \
    __TTK_BASE_STREAM__(level, msg); \
  }
// log stream condition base macro
#define __TTK_COND_STREAM__(cond, level, msg) \
  if(cond) \
  { \
    __TTK_BASE_STREAM__(level, msg); \
  }
// log stream count base macro
#define __TTK_COUNT_STREAM__(count, level, msg) \
  static int __last__ = 1; \
  if(count > 0 && ++__last__ > count) \
  { \
    __last__ = 1; \
    __TTK_BASE_STREAM__(level, msg); \
  }
// log stream period base macro
#define __TTK_PERIOD_STREAM__(period, level, msg) \
  static qint64 __last__ = 0; \
  const qint64 __now__ = QDateTime::currentMSecsSinceEpoch(); \
  if(__last__ + period * 1000 <= __now__ || __now__ < __last__) \
  { \
    __last__ = __now__; \
    __TTK_BASE_STREAM__(level, msg); \
  }


#define TTK_LOG_STREAM(msg) __TTK_LOG_OUTPUT__ << msg

#define TTK_TRACE_STREAM(msg) { __TTK_BASE_STREAM__("T", msg); }
#define TTK_DEBUG_STREAM(msg) { __TTK_BASE_STREAM__("D", msg); }
#define TTK_INFO_STREAM(msg)  { __TTK_BASE_STREAM__("I", msg); }
#define TTK_WARN_STREAM(msg)  { __TTK_BASE_STREAM__("W", msg); }
#define TTK_ERROR_STREAM(msg) { __TTK_BASE_STREAM__("E", msg); }
#define TTK_FATAL_STREAM(msg) { __TTK_BASE_STREAM__("F", msg); }

#define TTK_TRACE_STREAM_ONCE(msg) { __TTK_ONCE_STREAM__("T", msg) }
#define TTK_DEBUG_STREAM_ONCE(msg) { __TTK_ONCE_STREAM__("D", msg) }
#define TTK_INFO_STREAM_ONCE(msg)  { __TTK_ONCE_STREAM__("I", msg) }
#define TTK_WARN_STREAM_ONCE(msg)  { __TTK_ONCE_STREAM__("W", msg) }
#define TTK_ERROR_STREAM_ONCE(msg) { __TTK_ONCE_STREAM__("E", msg) }
#define TTK_FATAL_STREAM_ONCE(msg) { __TTK_ONCE_STREAM__("F", msg) }

#define TTK_TRACE_STREAM_COND(cond, msg) { __TTK_COND_STREAM__(cond, "T", msg) }
#define TTK_DEBUG_STREAM_COND(cond, msg) { __TTK_COND_STREAM__(cond, "D", msg) }
#define TTK_INFO_STREAM_COND(cond, msg)  { __TTK_COND_STREAM__(cond, "I", msg) }
#define TTK_WARN_STREAM_COND(cond, msg)  { __TTK_COND_STREAM__(cond, "W", msg) }
#define TTK_ERROR_STREAM_COND(cond, msg) { __TTK_COND_STREAM__(cond, "E", msg) }
#define TTK_FATAL_STREAM_COND(cond, msg) { __TTK_COND_STREAM__(cond, "F", msg) }

#define TTK_TRACE_STREAM_COUNT(count, msg) { __TTK_COUNT_STREAM__(count, "T", msg) }
#define TTK_DEBUG_STREAM_COUNT(count, msg) { __TTK_COUNT_STREAM__(count, "D", msg) }
#define TTK_INFO_STREAM_COUNT(count, msg)  { __TTK_COUNT_STREAM__(count, "I", msg) }
#define TTK_WARN_STREAM_COUNT(count, msg)  { __TTK_COUNT_STREAM__(count, "W", msg) }
#define TTK_ERROR_STREAM_COUNT(count, msg) { __TTK_COUNT_STREAM__(count, "E", msg) }
#define TTK_FATAL_STREAM_COUNT(count, msg) { __TTK_COUNT_STREAM__(count, "F", msg) }

#define TTK_TRACE_STREAM_PERIOD(period, msg) { __TTK_PERIOD_STREAM__(period, "T", msg) }
#define TTK_DEBUG_STREAM_PERIOD(period, msg) { __TTK_PERIOD_STREAM__(period, "D", msg) }
#define TTK_INFO_STREAM_PERIOD(period, msg)  { __TTK_PERIOD_STREAM__(period, "I", msg) }
#define TTK_WARN_STREAM_PERIOD(period, msg)  { __TTK_PERIOD_STREAM__(period, "W", msg) }
#define TTK_ERROR_STREAM_PERIOD(period, msg) { __TTK_PERIOD_STREAM__(period, "E", msg) }
#define TTK_FATAL_STREAM_PERIOD(period, msg) { __TTK_PERIOD_STREAM__(period, "F", msg) }

#endif // TTKLOGGER_H
