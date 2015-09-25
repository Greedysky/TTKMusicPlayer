#ifndef MUSICLOGGER_H
#define MUSICLOGGER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclibexportglobal.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>

#define CURRENTTIME QTime::currentTime().toString("hh:mm:ss:zzz")
#define CURRENTDATE QDate::currentDate().toString("yyyy-MM-dd")

#define M_LOOGER (*MusicLogger::createInstance())
#ifdef MUSIC_DEBUG
    #define M_LOOGERS(str) M_LOOGER << str
#else
    #define M_LOOGERS(str)
#endif

class MUSIC_CORE_EXPORT MusicLogger
{
public:
    static inline MusicLogger* createInstance()
    {
        static MusicLogger obj;
#ifdef MUSIC_DEBUG
        obj.stream() << QString("[%1 %2]:  %3").arg(CURRENTDATE)
                      .arg(CURRENTTIME).arg(obj.streamString()) << endl;
        obj.streamString().clear();
#endif
        return &obj;
    }

    ~MusicLogger()
    {
#ifdef MUSIC_DEBUG
        m_file.close();
#endif
    }

    QTextStream &stream() { return m_stream; }
    QString &streamString() { return m_streamString; }

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
    inline MusicLogger &operator<<(const char* t) { return debugData<const char*>(t); }
    inline MusicLogger &operator<<(const QString & t) { return debugData<QString>(t); }
    inline MusicLogger &operator<<(const QStringRef & t) { return debugData<QString>(t.toString()); }
    inline MusicLogger &operator<<(QLatin1String t) { return debugData<QLatin1String>(t); }
    inline MusicLogger &operator<<(const QByteArray & t) { return debugData<QString>(QString(t)); }


protected:
    MusicLogger()
    {
#ifdef MUSIC_DEBUG
        m_file.setFileName("logger.txt");
        m_file.open(QIODevice::WriteOnly | QIODevice::Append);
        m_stream.setDevice(&m_file);
        m_stream << QString().rightJustified(70, '=') << endl;
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

    Q_DISABLE_COPY(MusicLogger)
};

#endif // MUSICLOGGER_H
