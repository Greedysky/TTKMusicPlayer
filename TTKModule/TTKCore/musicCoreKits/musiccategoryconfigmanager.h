#ifndef MUSICCATEGORYCONFIGMANAGER_H
#define MUSICCATEGORYCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicabstractxml.h"

/*! @brief The class of the playlist category item.
 * @author Greedysky <greedysky@163.com>
 */
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
MUSIC_DECLARE_LISTS(MusicPlaylistCategoryItem)


/*! @brief The class of the playlist category core.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicPlaylistCategory
{
    QString m_category;
    MusicPlaylistCategoryItems m_items;
}MusicPlaylistCategory;
MUSIC_DECLARE_LISTS(MusicPlaylistCategory)


/*! @brief The class of the playlist category Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPlaylistCategoryConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlaylistCategoryConfigManager(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Read user datas from xml file by given name.
     */
    inline bool readCategoryConfig() { return readConfig(":/data/playlist"); }
    /*!
     * Read user datas into xml file.
     */
    void readCategoryConfig(MusicPlaylistCategorys &records, const QString &key);

};


/*! @brief The class of the toplist category Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicToplistCategoryConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicToplistCategoryConfigManager(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Read user datas from xml file by given name.
     */
    inline bool readCategoryConfig() { return readConfig(":/data/toplist"); }
    /*!
     * Read user datas into xml file.
     */
    void readCategoryConfig(MusicPlaylistCategorys &records, const QString &key);

};

#endif // MUSICCATEGORYCONFIGMANAGER_H
