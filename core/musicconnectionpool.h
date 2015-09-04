#ifndef MUSICCONNECTIONPOOL_H
#define MUSICCONNECTIONPOOL_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsingletone.h"

#define M_SETTING (MusicSingleton<MusicConnectionPool>::createInstance())

class MUSIC_CORE_EXPORT MusicConnectionPool : public QObject
{
    Q_OBJECT
public:
    explicit MusicConnectionPool(QObject *parent = 0);


protected:

    DECLARE_SINGLETON_CLASS(MusicConnectionPool)

};

#endif // MUSICCONNECTIONPOOL_H
