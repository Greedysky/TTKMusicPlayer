#ifndef MUSICLOGGER_H
#define MUSICLOGGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#ifndef MUSIC_MOBILE
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "musicglobaldefine.h"

#define CURRENTTIME QTime::currentTime().toString("hh:mm:ss:zzz")
#define CURRENTDATE QDate::currentDate().toString("yyyy-MM-dd")
#define LOG_END     QString("log::npos")

#define M_LOGGER    (*MusicLogger::createInstance())
#define M_MESSAGE(str, msg)         \
{                                   \
    M_LOGGER.setLevel(msg);         \
    M_LOGGER << str << LOG_END;     \
}

#ifdef TTK_DEBUG
    #define M_LOGGER_INFO(str)  M_MESSAGE(str, "[Info]")
    #define M_LOGGER_DEBUG(str) M_MESSAGE(str, "[Debug]")
    #define M_LOGGER_WARN(str)  M_MESSAGE(str, "[Warn]")
    #define M_LOGGER_TRACE(str) M_MESSAGE(str, "[Trace]")
    #define M_LOGGER_ERROR(str) M_MESSAGE(str, "[Error]")
    #define M_LOGGER_FATAL(str) M_MESSAGE(str, "[Fatal]")
#else
    #define M_LOGGER_INFO(str)  Q_UNUSED(str)
    #define M_LOGGER_DEBUG(str) Q_UNUSED(str)
    #define M_LOGGER_WARN(str)  Q_UNUSED(str)
    #define M_LOGGER_TRACE(str) Q_UNUSED(str)
    #define M_LOGGER_ERROR(str) Q_UNUSED(str)
    #define M_LOGGER_FATAL(str) Q_UNUSED(str)
#endif

/*! @brief The class of the application logger.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicLogger
{
public:
    /*!
     * Get object instance ptr.
     */
    inline static MusicLogger* createInstance()
    {
        static MusicLogger obj;
        return &obj;
    }

    /*!
     * Set logger level.
     */
    inline void setLevel(const QString &level)
    {
        m_levelType = level;
    }
    /*!
     * Get logger level.
     */
    inline QString level() const
    {
        return m_levelType;
    }

    /*!
     * Operator << override.
     */
    inline MusicLogger &operator <<(bool t)
    {
#ifdef TTK_DEBUG
        m_streamString.append( QString("%1 ").arg(t ? "true" : "false") );
#else
        Q_UNUSED(t);
#endif
        return *this;
    }
    inline MusicLogger &operator<<(char t) { return debugData<char>(t); }
    inline MusicLogger &operator<<(signed short t) { return debugData<short>(t);}
    inline MusicLogger &operator<<(ushort t) { return debugData<ushort>(t); }
    inline MusicLogger &operator<<(signed int t) { return debugData<int>(t); }
    inline MusicLogger &operator<<(uint t) { return debugData<uint>(t); }
    inline MusicLogger &operator<<(signed long t) { return debugData<long>(t); }
    inline MusicLogger &operator<<(ulong t) { return debugData<ulong>(t); }
    inline MusicLogger &operator<<(qint64 t) { return debugData<qint64>(t); }
    inline MusicLogger &operator<<(quint64 t) { return debugData<quint64>(t); }
    inline MusicLogger &operator<<(float t) { return debugData<float>(t); }
    inline MusicLogger &operator<<(double t) { return debugData<double>(t); }
    inline MusicLogger &operator<<(const char *t) { return debugData<const char*>(t); }
    inline MusicLogger &operator<<(const QString &t)
    {
#ifdef TTK_DEBUG
        if(t == LOG_END)
        {
            m_stream << QString("[%1 %2]:  %3").arg(CURRENTDATE).arg(CURRENTTIME).arg(m_streamString) << endl;
            m_streamString.clear();
        }
        else
        {
            debugData<QString>(t);
        }
#else
        Q_UNUSED(t);
#endif
        return *this;
    }
    inline MusicLogger &operator<<(const QStringRef &t) { return debugData<QString>(t.toString()); }
    inline MusicLogger &operator<<(const QLatin1String &t) { return debugData<QLatin1String>(t); }
    inline MusicLogger &operator<<(const QByteArray &t) { return debugData<QString>(QString(t)); }

private:
    /*!
     * Object contsructor.
     */
    MusicLogger()
    {
#ifdef TTK_DEBUG
        m_file.setFileName("logger.txt");
        m_file.open(QIODevice::WriteOnly | QIODevice::Append);
        m_stream.setDevice(&m_file);
        m_stream << QString().rightJustified(70, '=') << endl;
        m_levelType = "[Info]";
#endif
    }

    ~MusicLogger()
    {
#ifdef TTK_DEBUG
        m_file.close();
#endif
    }

    /*!
     * Output debug data into local file.
     */
    template <typename T>
    MusicLogger &debugData(const T &data)
    {
#ifdef TTK_DEBUG
        m_streamString.append( QString("%1 %2 ").arg(m_levelType).arg(data) );
#else
        Q_UNUSED(data);
#endif
        return *this;
    }

    QTextStream m_stream;
    QString m_streamString;
    QString m_levelType;
    QFile m_file;

};
#else
    #define M_LOGGER_INFO(str)  Q_UNUSED(str)
    #define M_LOGGER_DEBUG(str) Q_UNUSED(str)
    #define M_LOGGER_WARN(str)  Q_UNUSED(str)
    #define M_LOGGER_TRACE(str) Q_UNUSED(str)
    #define M_LOGGER_ERROR(str) Q_UNUSED(str)
    #define M_LOGGER_FATAL(str) Q_UNUSED(str)
#endif

#endif // MUSICLOGGER_H
