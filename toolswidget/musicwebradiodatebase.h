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
    QString getRadioUrl(const QString &channelName);
    QStringList getRadioNames(const QString &category);
    QStringList getRecentNames();
    QStringList getFavouriteNames();
    void radioRecentPlay(const QString &channelName);
    void radioCollection(const QString &name);
    void radioDiscollection(const QString &name);

protected:
    QStringList getRecords(const QString &filter);

};

#endif // MUSICWEBRADDATEBASE_H
