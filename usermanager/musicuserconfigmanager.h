#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

typedef struct MusicUserRecord
{
    QStringList m_names;
    QStringList m_rps;
    QStringList m_als;
    QStringList m_pwds;
}MusicUserRecord;

class MUSIC_USER_EXPORT MusicUserConfigManager : public MusicAbstractXml
{
public:
    explicit MusicUserConfigManager(QObject *parent = 0);

    //UserXMLConfig
    inline bool readUserXMLConfig(){ return readConfig(USERPATH); }
    void writeUserXMLConfig(const MusicUserRecord &record);
    void readUserConfig(MusicUserRecord &record);

};

#endif // MUSICXMLCONFIGMANAGER_H
