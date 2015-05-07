#ifndef MUSICLOGGER_H
#define MUSICLOGGER_H

#include "musicobject.h"
#include "musiclibexportglobal.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#define CURRENTTIME QTime::currentTime().toString("hh:mm:ss:zzz")
#define CURRENTDATE QDate::currentDate().toString("yyyy-MM-dd")

#ifdef MUSIC_DEBUG
    #define mlogger(str) (MusicLogger::createInstance())<<str
#else
    #define mlogger(str)
#endif

class MusicLogger
{

public:
    ~MusicLogger()
    {
        file.close();
    }
    static MusicLogger* createInstance()
    {
        static MusicLogger obj;
        return obj;
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

};

MusicLogger* MusicLogger::obj = NULL;

#endif // MUSICLOGGER_H
