#ifndef MUSICFMCONFIGMANAGER_H
#define MUSICFMCONFIGMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "musicobject.h"
#include "ttkabstractxml.h"

/*! @brief The class of the fm channel item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicFMChannel
{
    QString m_name;
    QString m_url;
};
TTK_DECLARE_LIST(MusicFMChannel);


/*! @brief The class of the fm category item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicFMCategory
{
    QString m_category;
    MusicFMChannelList m_items;
};
TTK_DECLARE_LIST(MusicFMCategory);

/*! @brief The class of the fm config manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFMConfigManager : public TTKAbstractXml, public TTKAbstractReadWriteInterface<MusicFMCategoryList>
{
public:
    /*!
     * Object constructor.
     */
    MusicFMConfigManager();

    /*!
     * Read datas from buffer.
     */
    virtual bool readBuffer(MusicFMCategoryList &items) override final;
    /*!
     * Write datas into buffer.
     */
    virtual bool writeBuffer(const MusicFMCategoryList &items) override final;

};

#endif // MUSICFMCONFIGMANAGER_H
