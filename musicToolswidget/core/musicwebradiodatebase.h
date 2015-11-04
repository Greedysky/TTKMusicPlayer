#ifndef MUSICWEBRADDATEBASE_H
#define MUSICWEBRADDATEBASE_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musiclibexportglobal.h"

class MUSIC_TOOL_EXPORT MusicWebRadioDatabase
{

public:
    MusicWebRadioDatabase();

    bool connectDatabase() const;
    bool disConnectDatabase() const;
    QString getRadioUrl(const QString &channelName) const;
    QStringList getRadioNames(const QString &category) const;
    QStringList getRecentNames() const;
    QStringList getFavouriteNames() const;
    void radioRecentPlay(const QString &channelName) const;
    void radioCollection(const QString &name) const;
    void radioDiscollection(const QString &name) const;

protected:
    QStringList getRecords(const QString &filter) const;

};

#endif // MUSICWEBRADDATEBASE_H
