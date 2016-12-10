#ifndef MUSICLOGGER_H
#define MUSICLOGGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#ifndef MUSIC_MOBILE
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "musicglobaldefine.h"

#define CURRENTTIME QTime::currentTime().toString("hh:mm:ss:zzz")
#define CURRENTDATE QDate::currentDate().toString("yyyy-MM-dd")
#define LOG_END     QString("log::npos")

#define M_LOGGER    (*MusicLogger::createInstance())
#define M_MESSAGE(str, msg) \
{ \
    M_LOGGER.setLevel(msg); \
    M_LOGGER << str << LOG_END;\
}

#ifdef MUSIC_DEBUG
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
    static inline MusicLogger* createInstance()
    {
        static MusicLogger obj;
        return &obj;
    }
    /*!
     * Get object instance ptr.
     */

    static QString getClassName()
    {
        return "MusicLogger";
    }
    /*!
     * Get class object name.
     */
    inline void setLevel(const QString &level)
    {
        m_levelType = level;
    }
    /*!
     * Set logger level.
     */
    inline QString level() const
    {
        return m_levelType;
    }
    /*!
     * Get logger level.
     */

    inline MusicLogger &operator <<(bool t)
    {
#ifdef MUSIC_DEBUG
        m_streamString.append( QString("%1 ").arg(t ? "true" : "false") );
#else
        Q_UNUSED(t);
#endif
        return *this;
    }
    inline MusicLogger &operator<<(char t) { return debugData<char>(t); }
    inline MusicLogger &operator<<(signed short t) { return debugData<short>(t);}
    inline MusicLogger &operator<<(unsigned short t) { return debugData<ushort>(t); }
    inline MusicLogger &operator<<(signed int t) { return debugData<int>(t); }
    inline MusicLogger &operator<<(unsigned int t) { return debugData<uint>(t); }
    inline MusicLogger &operator<<(signed long t) { return debugData<long>(t); }
    inline MusicLogger &operator<<(unsigned long t) { return debugData<ulong>(t); }
    inline MusicLogger &operator<<(qint64 t) { return debugData<qint64>(t); }
    inline MusicLogger &operator<<(quint64 t) { return debugData<quint64>(t); }
    inline MusicLogger &operator<<(float t) { return debugData<float>(t); }
    inline MusicLogger &operator<<(double t) { return debugData<double>(t); }
    inline MusicLogger &operator<<(const char *t) { return debugData<const char*>(t); }
    inline MusicLogger &operator<<(const QString &t)
    {
#ifdef MUSIC_DEBUG
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
    /*!
     * Operator << override.
     */

private:
    MusicLogger()
    {
#ifdef MUSIC_DEBUG
        m_file.setFileName("logger.txt");
        m_file.open(QIODevice::WriteOnly | QIODevice::Append);
        m_stream.setDevice(&m_file);
        m_stream << QString().rightJustified(70, '=') << endl;
        m_levelType = "[Info]";
#endif
    }
    /*!
     * Object contsructor.
     */

    ~MusicLogger()
    {
#ifdef MUSIC_DEBUG
        m_file.close();
#endif
    }

    template <class T>
    MusicLogger &debugData(const T &data)
    {
#ifdef MUSIC_DEBUG
        m_streamString.append( QString("%1 %2 ").arg(m_levelType).arg(data) );
#else
        Q_UNUSED(data);
#endif
        return *this;
    }
    /*!
     * Output debug data into local file.
     */

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
