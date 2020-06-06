#ifndef TTKLOGGER_H
#define TTKLOGGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QTextStream>

#define CURRENT_TIME QTime::currentTime().toString(MUSIC_ZTIME_FORMAT)
#define CURRENT_DATE QDate::currentDate().toString(MUSIC_YEAR_FORMAT)
#define LOG_END      QString("log::npos")

#define TTK_LOGGER    (*TTKLogger::createInstance())
#define TTK_MESSAGE(str, msg)       \
{                                   \
    TTK_LOGGER.setLevel(msg);       \
    TTK_LOGGER << str << LOG_END;   \
}

#ifdef TTK_DEBUG
    #define TTK_LOGGER_INFO(str)  TTK_MESSAGE(str, "[Info]")
    #define TTK_LOGGER_DEBUG(str) TTK_MESSAGE(str, "[Debug]")
    #define TTK_LOGGER_WARN(str)  TTK_MESSAGE(str, "[Warn]")
    #define TTK_LOGGER_TRACE(str) TTK_MESSAGE(str, "[Trace]")
    #define TTK_LOGGER_ERROR(str) TTK_MESSAGE(str, "[Error]")
    #define TTK_LOGGER_FATAL(str) TTK_MESSAGE(str, "[Fatal]")
#else
    #define TTK_LOGGER_INFO(str)  qDebug() << str
    #define TTK_LOGGER_DEBUG(str) qDebug() << str
    #define TTK_LOGGER_WARN(str)  qDebug() << str
    #define TTK_LOGGER_TRACE(str) qDebug() << str
    #define TTK_LOGGER_ERROR(str) qDebug() << str
    #define TTK_LOGGER_FATAL(str) qDebug() << str
#endif

/*! @brief The class of the application logger.
 * @author Greedysky <greedysky@163.com>
 */
class Q_DECL_EXPORT TTKLogger
{
public:
    /*!
     * Get object instance ptr.
     */
    inline static TTKLogger* createInstance()
    {
        static TTKLogger obj;
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
    inline TTKLogger &operator <<(bool t)
    {
#ifdef TTK_DEBUG
        m_streamString.append(QString("%1 ").arg(t ? "true" : "false"));
#else
        Q_UNUSED(t);
#endif
        return *this;
    }
    inline TTKLogger &operator<<(char t) { return debugData<char>(t); }
    inline TTKLogger &operator<<(signed short t) { return debugData<short>(t); }
    inline TTKLogger &operator<<(ushort t) { return debugData<ushort>(t); }
    inline TTKLogger &operator<<(signed int t) { return debugData<int>(t); }
    inline TTKLogger &operator<<(uint t) { return debugData<uint>(t); }
    inline TTKLogger &operator<<(signed long t) { return debugData<long>(t); }
    inline TTKLogger &operator<<(ulong t) { return debugData<ulong>(t); }
    inline TTKLogger &operator<<(qint64 t) { return debugData<qint64>(t); }
    inline TTKLogger &operator<<(quint64 t) { return debugData<quint64>(t); }
    inline TTKLogger &operator<<(float t) { return debugData<float>(t); }
    inline TTKLogger &operator<<(double t) { return debugData<double>(t); }
    inline TTKLogger &operator<<(const char *t) { return debugData<const char*>(t); }
    inline TTKLogger &operator<<(const QStringRef &t) { return debugData<QString>(t.toString()); }
    inline TTKLogger &operator<<(const QLatin1String &t) { return debugData<QLatin1String>(t); }
    inline TTKLogger &operator<<(const QByteArray &t) { return debugData<QString>(QString(t)); }
    inline TTKLogger &operator<<(const QString &t)
    {
#ifdef TTK_DEBUG
        if(t == LOG_END)
        {
            m_stream << QString("[%1 %2]:  %3").arg(CURRENT_DATE).arg(CURRENT_TIME).arg(m_streamString);
#if TTK_QT_VERSION_CHECK(5,15,0)
            m_stream << Qt::endl;
#else
            m_stream << endl;
#endif
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

private:
    /*!
     * Object contsructor.
     */
    TTKLogger()
    {
#ifdef TTK_DEBUG
        m_file.setFileName("logger.txt");
        m_file.open(QIODevice::WriteOnly | QIODevice::Append);
        m_stream.setDevice(&m_file);
        m_stream << QString().rightJustified(70, '=');
#if TTK_QT_VERSION_CHECK(5,15,0)
        m_stream << Qt::endl;
#else
        m_stream << endl;
#endif
        m_levelType = "[Info]";
#endif
    }

    ~TTKLogger()
    {
#ifdef TTK_DEBUG
        m_file.close();
#endif
    }

    /*!
     * Output debug data into local file.
     */
    template <typename T>
    TTKLogger &debugData(const T &data)
    {
#ifdef TTK_DEBUG
        m_streamString.append(QString("%1 %2 ").arg(m_levelType).arg(data));
#else
        Q_UNUSED(data);
#endif
        return *this;
    }

    QFile m_file;
    QTextStream m_stream;
    QString m_streamString;
    QString m_levelType;

};

#endif // TTKLOGGER_H
