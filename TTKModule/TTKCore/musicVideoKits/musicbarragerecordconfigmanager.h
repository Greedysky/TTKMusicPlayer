#ifndef MUSICBARRAGERECORDCONFIGMANAGER_H
#define MUSICBARRAGERECORDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

/*! @brief The class of the barrage record item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_SUPER_EXPORT MusicBarrageRecord
{
    int m_size;
    QString m_color;
    QString m_value;
}MusicBarrageRecord;
MUSIC_DECLARE_LISTS(MusicBarrageRecord)

/*! @brief The class of the barrage record manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SUPER_EXPORT MusicBarrageRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBarrageRecordConfigManager(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Read barrage datas from xml file by given name.
     */
    inline bool readBarrageXMLConfig(){ return readConfig(BARRAGEPATH_FULL); }
    /*!
     * Write barrage datas into xml file.
     */
    void writeBarrageConfig(const MusicBarrageRecords &records);
    /*!
     * Read barrage datas into xml file.
     */
    void readBarrageConfig(MusicBarrageRecords &records);

};

#endif // MUSICBARRAGERECORDCONFIGMANAGER_H
