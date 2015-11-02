#ifndef MUSICLOGGER_H
#define MUSICLOGGER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "musiclibexportglobal.h"

#define CURRENTTIME QTime::currentTime().toString("hh:mm:ss:zzz")
#define CURRENTDATE QDate::currentDate().toString("yyyy-MM-dd")
#define LOG_END  QString("log::npos")

#define M_LOGGER (*MusicLogger::createInstance())

#ifdef MUSIC_DEBUG
    #define M_LOGGERS(str) M_LOGGER << str << LOG_END
#else
    #define M_LOGGERS(str)
#endif

class MUSIC_CORE_EXPORT MusicLogger
{
public:
    static inline MusicLogger* createInstance()
    {
        static MusicLogger obj;
        return &obj;
    }

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

private:
    MusicLogger()
    {
#ifdef MUSIC_DEBUG
        m_file.setFileName("logger.txt");
        m_file.open(QIODevice::WriteOnly | QIODevice::Append);
        m_stream.setDevice(&m_file);
        m_stream << QString().rightJustified(70, '=') << endl;
#endif
    }

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
        m_streamString.append( QString("%1 ").arg(data) );
#else
        Q_UNUSED(data);
#endif
        return *this;
    }

    QTextStream m_stream;
    QString m_streamString;
    QFile m_file;

};

#endif // MUSICLOGGER_H
