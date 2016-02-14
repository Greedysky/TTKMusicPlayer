#ifndef MUSICDOWNLOADQUERYSINGLETHREAD_H
#define MUSICDOWNLOADQUERYSINGLETHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

class MUSIC_NETWORK_EXPORT MusicDownLoadQuerySingleThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQuerySingleThread(QObject *parent = 0);
    virtual ~MusicDownLoadQuerySingleThread();

    virtual void startSearchSong(QueryType type, const QString &text) override;

public Q_SLOTS:
    virtual void searchFinshed() override;

};

#endif // MUSICDOWNLOADQUERYSINGLETHREAD_H
