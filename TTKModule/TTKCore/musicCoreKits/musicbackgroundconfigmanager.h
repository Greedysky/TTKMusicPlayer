#ifndef MUSICBACKGROUNDCONFIGMANAGER_H
#define MUSICBACKGROUNDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include <QPixmap>
#include "musicabstractxml.h"
#include "musicotherdefine.h"

/*! @brief The class of the skin config item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicSkinConfigItem
{
    QString m_name;
    int m_useCount;

    MusicSkinConfigItem()
    {
        m_name = MUSIC_AUTHOR_NAME;
        m_useCount = 8888;
    }

    bool isValid() const
    {
        return !(m_name.isEmpty() && m_useCount == 0);
    }

}MusicSkinConfigItem;
TTK_DECLARE_LISTS(MusicSkinConfigItem)


/*! @brief The class of the skin backgroud image item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicBackgroundImage
{
    QPixmap m_pix;
    MusicSkinConfigItem m_item;

    bool isValid() const
    {
        return !m_pix.isNull() && m_item.isValid();
    }

}MusicBackgroundImage;
TTK_DECLARE_LISTS(MusicBackgroundImage)


/*! @brief The class of the skin XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSkinConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSkinConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSkinConfigManager(QObject *parent = nullptr);

    /*!
     * Read datas from config file.
     */
    void readSkinData(MusicSkinConfigItem &item);
    /*!
     * Write datas into config file.
     */
    void writeSkinData(const MusicSkinConfigItem &item, const QString &path);

};

#endif // MUSICBACKGROUNDCONFIGMANAGER_H
