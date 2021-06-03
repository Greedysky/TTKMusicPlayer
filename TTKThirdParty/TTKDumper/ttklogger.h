#ifndef TTKLOGGER_H
#define TTKLOGGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QDebug>
#include <QDateTime>

#define CURRENT_TIME QTime::currentTime().toString("hh:mm:ss:zzz")
#define CURRENT_DATE QDate::currentDate().toString("yyyy-MM-dd")

#define TTK_LOGGER_INFO(str)  qDebug() << QString("[Info %1 %2]: ").arg(CURRENT_DATE, CURRENT_TIME) << str
#define TTK_LOGGER_DEBUG(str) qDebug() << QString("[Debug %1 %2]: ").arg(CURRENT_DATE, CURRENT_TIME) << str
#define TTK_LOGGER_WARN(str)  qDebug() << QString("[Warn %1 %2]: ").arg(CURRENT_DATE, CURRENT_TIME) << str
#define TTK_LOGGER_TRACE(str) qDebug() << QString("[Trace %1 %2]: ").arg(CURRENT_DATE, CURRENT_TIME) << str
#define TTK_LOGGER_ERROR(str) qDebug() << QString("[Error %1 %2]: ").arg(CURRENT_DATE, CURRENT_TIME) << str
#define TTK_LOGGER_FATAL(str) qDebug() << QString("[Fatal %1 %2]: ").arg(CURRENT_DATE, CURRENT_TIME) << str

#endif // TTKLOGGER_H
