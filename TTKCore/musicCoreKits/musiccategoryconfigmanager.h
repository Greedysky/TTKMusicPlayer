#ifndef MUSICCATEGORYCONFIGMANAGER_H
#define MUSICCATEGORYCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

typedef struct MUSIC_CORE_EXPORT MusicPlaylistCategoryItem
{
    QString m_id;
    QString m_name;

    MusicPlaylistCategoryItem()
    {

    }

    MusicPlaylistCategoryItem(const QString &id, const QString &name)
    {
        m_id = id;
        m_name = name;
    }
}MusicPlaylistCategoryItem;
TTK_DECLARE_LISTS(MusicPlaylistCategoryItem)

typedef struct MUSIC_CORE_EXPORT MusicPlaylistCategory
{
    QString m_category;
    MusicPlaylistCategoryItems m_items;
}MusicPlaylistCategory;
TTK_DECLARE_LISTS(MusicPlaylistCategory)

/*! @brief The class of the category Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicCategoryConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicCategoryConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline bool readCategoryConfig() { return readConfig(":/data/playlist"); }
    /*!
     * Read user datas from xml file by given name.
     */
    void readCategoryConfig(MusicPlaylistCategorys &records, const QString &key);
    /*!
     * Read user datas into xml file.
     */

};

#endif // MUSICCATEGORYCONFIGMANAGER_H
