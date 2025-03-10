#ifndef MUSICIDENTIFYSONGREQUEST_H
#define MUSICIDENTIFYSONGREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicabstractnetwork.h"

/*! @brief The class of the song identify data item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongIdentifyData
{
    QString m_songName;
    QString m_artistName;
};
TTK_DECLARE_LIST(MusicSongIdentifyData);

/*! @brief The class of the abstract song identify query request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractIdentifyRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractIdentifyRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractIdentifyRequest(QObject *parent = nullptr);

    /*!
     * Get query cloud id keys.
     */
    bool queryCloudKey();

    /*!
     * Start to download identify data.
     */
    virtual void startToRequest(const QString &path) = 0;

    /*!
     * Get identify songs.
     */
    inline const MusicSongIdentifyDataList& items() const noexcept { return m_items; }

Q_SIGNALS:
    /*!
     * Get key data from net finished.
     */
    void finished();

protected Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadKeyFinished(const QByteArray &bytes) = 0;

protected:
    MusicSongIdentifyDataList m_items;
    QString m_accessKey, m_accessSecret;

};


/*! @brief The class of the acr song identify query request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicACRIdentifyRequest : public MusicAbstractIdentifyRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicACRIdentifyRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicACRIdentifyRequest(QObject *parent = nullptr);

    /*!
     * Start to download identify data.
     */
    virtual void startToRequest(const QString &path) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadKeyFinished(const QByteArray &bytes) override final;

};


/*! @brief The class of the xf song identify query request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicXFIdentifyRequest : public MusicAbstractIdentifyRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicXFIdentifyRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicXFIdentifyRequest(QObject *parent = nullptr);

    /*!
     * Start to download identify data.
     */
    virtual void startToRequest(const QString &path) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadKeyFinished(const QByteArray &bytes) override final;

};

#endif // MUSICIDENTIFYSONGREQUEST_H
