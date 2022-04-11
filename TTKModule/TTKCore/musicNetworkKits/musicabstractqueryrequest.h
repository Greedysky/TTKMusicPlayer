#ifndef MUSICABSTRACTQUERYREQUEST_H
#define MUSICABSTRACTQUERYREQUEST_H

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

#include "musicstringutils.h"
#include "musicpagequeryrequest.h"

/*! @brief The class of the searched data item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSearchedItem
{
    QString m_songName;
    QString m_singerName;
    QString m_albumName;
    QString m_duration;
    QString m_type;

    MusicSearchedItem()
    {
        m_songName = TTK_DEFAULT_STR;
        m_singerName = TTK_DEFAULT_STR;
        m_albumName = TTK_DEFAULT_STR;
        m_duration = TTK_DEFAULT_STR;
        m_type = TTK_DEFAULT_STR;
    }
};

/*! @brief The class of the search song results data item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicResultsItem
{
    QString m_id;
    QString m_name;
    QString m_nickName;
    QString m_coverUrl;
    QString m_playCount;
    QString m_description;
    QString m_updateTime;
    QString m_tags;

    MusicResultsItem()
    {
        m_id = TTK_DEFAULT_STR;
        m_name = TTK_DEFAULT_STR;
        m_nickName = TTK_DEFAULT_STR;
        m_coverUrl = TTK_DEFAULT_STR;
        m_playCount = TTK_DEFAULT_STR;
        m_description = TTK_DEFAULT_STR;
        m_updateTime = TTK_DEFAULT_STR;
        m_tags = TTK_DEFAULT_STR;
    }

    inline bool isEmpty() const
    {
        return m_name == TTK_DEFAULT_STR || m_nickName == TTK_DEFAULT_STR || m_coverUrl == TTK_DEFAULT_STR || m_description == TTK_DEFAULT_STR || m_tags == TTK_DEFAULT_STR;
    }
};
TTK_DECLARE_LIST(MusicResultsItem);

#define QUERY_WY_INTERFACE      "WangYi"
#define QUERY_QQ_INTERFACE      "QQ"
#define QUERY_BD_INTERFACE      "BaiDu"
#define QUERY_KW_INTERFACE      "KuWo"
#define QUERY_KG_INTERFACE      "KuGou"

enum DownloadQueryServer
{
    WYQueryServer,
    QQQueryServer,
    KWQueryServer,
    KGQueryServer
};

/*! @brief The class of abstract query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractQueryRequest : public MusicPageQueryRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractQueryRequest)
public:
    enum QueryType
    {
        MusicQuery,     /*!< query music*/
        MovieQuery,     /*!< query movie*/
        LrcQuery,       /*!< query lrc*/
        OtherQuery,     /*!< query external*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicAbstractQueryRequest(QObject *parent = nullptr);

    /*!
     * Start to search data from name and type.
     * Subclass should implement this function.
     */
    virtual void startToSearch(QueryType type, const QString &value) = 0;
    /*!
     * Start to search single data from id.
     * Subclass should implement this function.
     */
    virtual void startToSingleSearch(const QString &value);

    /*!
     * Set query data quality.
     */
    inline void setQueryQuality(const QString &qual) { m_queryQuality = qual; }
    /*!
     * Get query data quality.
     */
    inline QString queryQuality() const { return m_queryQuality; }
    /*!
     * Set wheather query all quality of records.
     */
    inline void setQueryAllRecords(bool state) { m_queryAllRecords = state; }
    /*!
     * Get query all records flag.
     */
    inline bool queryAllRecords() const { return m_queryAllRecords; }
    /*!
     * Set query simplify mode.
     */
    inline void setQuerySimplify(bool state) { m_querySimplify = state; }
    /*!
     * Get query simplify flag.
     */
    inline bool querySimplify() const { return m_querySimplify; }
    /*!
     * Return the current song query type.
     */
    inline void setQueryType(QueryType type) { m_queryType = type; }
    /*!
     * Return the current song query type.
     */
    inline QueryType queryType() const { return m_queryType; }
    /*!
     * Return the current song query server.
     */
    inline QString queryServer() const { return m_queryServer; }
    /*!
     * Return the current song name.
     */
    inline QString queryValue() const { return m_queryValue; }
    /*!
     * Set the current song container.
     */
    inline void setMusicSongInfoList(const MusicObject::MusicSongInformationList &infos) { m_musicSongInfos = infos; }
    /*!
     * Get the current song container.
     */
    inline const MusicObject::MusicSongInformationList& musicSongInfoList() const { return m_musicSongInfos; }
    /*!
     * Check the current song container is empty.
     */
    inline bool isEmpty() const { return m_musicSongInfos.isEmpty(); }
    /*!
     * Map query server string.
     */
    QString mapQueryServerString() const;

public:
    /*!
     * Get download file size.
     */
    static qint64 fileSizeByUrl(const QString &url);

Q_SIGNALS:
    /*!
     * Clear all items before the new query start.
     */
    void clearAllItems();
    /*!
     * Create the current items by song name\ artist name and time.
     */
    void createSearchedItem(const MusicSearchedItem &songItem);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    /*!
     * Find download file size.
     */
    bool findUrlFileSize(MusicObject::MusicSongProperty *prop) const;
    /*!
     * Find download file size.
     */
    bool findUrlFileSize(MusicObject::MusicSongPropertyList *props) const;

    QString m_queryValue;
    QString m_queryServer;
    QString m_queryQuality;
    QueryType m_queryType;
    bool m_queryAllRecords, m_querySimplify;
    MusicObject::MusicSongInformationList m_musicSongInfos;

};

#endif // MUSICABSTRACTQUERYREQUEST_H
