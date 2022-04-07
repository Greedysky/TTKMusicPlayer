#ifndef MUSICCATEGORYCONFIGMANAGER_H
#define MUSICCATEGORYCONFIGMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
 ***************************************************************************/

#include "musicabstractxml.h"

/*! @brief The class of the results category item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicResultsCategoryItem
{
    QString m_key;
    QString m_value;
};
TTK_DECLARE_LIST(MusicResultsCategoryItem);


/*! @brief The class of the results category core.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicResultsCategory
{
    QString m_category;
    MusicResultsCategoryItemList m_items;
};
TTK_DECLARE_LIST(MusicResultsCategory);


/*! @brief The class of the category config manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCategoryConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCategoryConfigManager)
public:
    enum Type
    {
        Playlist,        /*!< Playlist Category*/
        Toplist,         /*!< Toplist Category*/
        ArtistList,      /*!< ArtistList Category*/
        MovieList,       /*!< MovieList Category*/
    };
    /*!
     * Object contsructor.
     */
    explicit MusicCategoryConfigManager(QObject *parent = nullptr);

    /*!
     * Read datas from xml file by given name.
     */
    bool fromFile(Type type);

    /*!
     * Read datas from config file.
     */
    void readBuffer(MusicResultsCategoryList &items, const QString &key);

};

#endif // MUSICCATEGORYCONFIGMANAGER_H
