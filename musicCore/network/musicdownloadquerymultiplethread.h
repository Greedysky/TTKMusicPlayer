#ifndef MUSICDOWNLOADQUERYMULTIPLETHREAD_H
#define MUSICDOWNLOADQUERYMULTIPLETHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

class MUSIC_NETWORK_EXPORT MusicDownLoadQueryMultipleThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryMultipleThread(QObject *parent = 0);
    virtual ~MusicDownLoadQueryMultipleThread();

    virtual void startSearchSong(QueryType type, const QString &text) override;

public Q_SLOTS:
    virtual void searchFinshed() override;

protected:
    QString getCurrentURL() const;
    void readFromMusicSongAttribute(MusicSongInfomation &info, const QString &size,
                                    int bit, const QString &url);

};

#endif // MUSICDOWNLOADQUERYMULTIPLETHREAD_H
