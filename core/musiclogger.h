#ifndef MUSICLOGGER_H
#define MUSICLOGGER_H

#include "musicsingletone.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>

#define CURRENTTIME QTime::currentTime().toString("hh:mm:ss:zzz")
#define CURRENTDATE QDate::currentDate().toString("yyyy-MM-dd")

#ifdef MUSIC_DEBUG
    #define M_LOOGER(str) (MusicSingleton<MusicLogger>::createInstance())<<str
#else
    #define M_LOOGER(str)
#endif

class MUSIC_CORE_EXPORT MusicLogger
{

public:
    ~MusicLogger()
    {
        file.close();
    }
    inline MusicLogger &operator<<(const QString &str)
    {
        QTextStream stream(&file);
        stream<<QString("[%1 %2]:  %3").arg(CURRENTDATE)
                .arg(CURRENTTIME).arg(str)<<endl;
        return *this;
    }

protected:
    MusicLogger()
    {
        file.setFileName("logger.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream stream(&file);
        stream<<QString().rightJustified(70,'=')<<endl;
    }
    QFile file;

    DECLARE_SINGLETON_CLASS(MusicLogger)
};

#endif // MUSICLOGGER_H
