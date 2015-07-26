#ifndef MUSICWEBRADDATEBASE_H
#define MUSICWEBRADDATEBASE_H

#include "musicobject.h"
#include "musiclibexportglobal.h"

class MUSIC_CORE_EXPORT MusicWebRadioDatabase
{

public:
    MusicWebRadioDatabase();
    bool connectDatabase();
    bool disConnectDatabase();
    QString getRadioUrl(const QString&);
    QStringList getRadioNames(const QString&);
    QStringList getRecentNames();
    QStringList getFavouriteNames();
    void radioRecentPlay(const QString&);
    void radioCollection(const QString&);
    void radioDiscollection(const QString&);

protected:
    QStringList getRecords(const QString& filter);

};

#endif // MUSICWEBRADDATEBASE_H
