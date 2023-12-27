#ifndef MUSICABSTRACTDOWNLOADSKINREQUEST_H
#define MUSICABSTRACTDOWNLOADSKINREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "ttkabstractxml.h"
#include "musicglobaldefine.h"

/*! @brief The class of the remote skin item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSkinRemoteItem
{
    QString m_name;
    QString m_url;
    int m_useCount;
    int m_index;

    MusicSkinRemoteItem()
        : m_name(MUSIC_AUTHOR_NAME),
          m_useCount(8888),
          m_index(0)
    {

    }

    inline bool isValid() const
    {
        return !m_name.isEmpty();
    }
};
TTK_DECLARE_LIST(MusicSkinRemoteItem);


/*! @brief The class of the remote skin item group.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSkinRemoteGroup
{
    QString m_group;
    QString m_id;
    MusicSkinRemoteItemList m_items;

    inline bool isValid() const
    {
        return !(m_group.isEmpty() && m_id.isEmpty() && m_items.isEmpty());
    }
};
TTK_DECLARE_LIST(MusicSkinRemoteGroup);


/*! @brief The class of the download skin remote background.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractDownloadSkinRequest : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractDownloadSkinRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractDownloadSkinRequest(QObject *parent = nullptr);

    /*!
     * Start to download skin data.
     */
    virtual void startRequest();
    /*!
     * Start to download skin data.
     */
    virtual void startRequest(const QString &id);

Q_SIGNALS:
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged(const MusicSkinRemoteGroupList &bytes);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     * Subclass should implement this function.
     */
    virtual void downLoadFinished(const QByteArray &bytes) = 0;

};

#endif // MUSICABSTRACTDOWNLOADSKINREQUEST_H
