#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

typedef struct MUSIC_USER_EXPORT MusicUserRecord
{
    QStringList m_names;
    QStringList m_rps;
    QStringList m_als;
    QStringList m_pwds;
}MusicUserRecord;

/*! @brief The class of the user config manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicUserConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline bool readUserXMLConfig(){ return readConfig(USERPATH_FULL); }
    /*!
     * Read user datas from xml file by given name.
     */
    void writeUserXMLConfig(const MusicUserRecord &record);
    /*!
     * Write user datas into xml file.
     */
    void readUserConfig(MusicUserRecord &record);
    /*!
     * Read user datas into xml file.
     */

};

#endif // MUSICXMLCONFIGMANAGER_H
