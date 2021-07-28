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

#if QT_VERSION < QT_VERSION_CHECK(5,4,0)
#define TTK_LOGGER_OUTPUT qDebug()
#else
#define TTK_LOGGER_OUTPUT qDebug().noquote()
#endif

#define TTK_LOGGER_INFO(str)  TTK_LOGGER_OUTPUT << QString("[%1 %2][I][%3(%4)] ").arg(CURRENT_DATE, CURRENT_TIME).arg(__FILE__).arg(__LINE__) << str
#define TTK_LOGGER_DEBUG(str) TTK_LOGGER_OUTPUT << QString("[%1 %2][D][%3(%4)] ").arg(CURRENT_DATE, CURRENT_TIME).arg(__FILE__).arg(__LINE__) << str
#define TTK_LOGGER_WARN(str)  TTK_LOGGER_OUTPUT << QString("[%1 %2][W][%3(%4)] ").arg(CURRENT_DATE, CURRENT_TIME).arg(__FILE__).arg(__LINE__) << str
#define TTK_LOGGER_TRACE(str) TTK_LOGGER_OUTPUT << QString("[%1 %2][T][%3(%4)] ").arg(CURRENT_DATE, CURRENT_TIME).arg(__FILE__).arg(__LINE__) << str
#define TTK_LOGGER_ERROR(str) TTK_LOGGER_OUTPUT << QString("[%1 %2][E][%3(%4)] ").arg(CURRENT_DATE, CURRENT_TIME).arg(__FILE__).arg(__LINE__) << str
#define TTK_LOGGER_FATAL(str) TTK_LOGGER_OUTPUT << QString("[%1 %2][F][%3(%4)] ").arg(CURRENT_DATE, CURRENT_TIME).arg(__FILE__).arg(__LINE__) << str

#endif // TTKLOGGER_H
