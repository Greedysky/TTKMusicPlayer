#ifndef MUSICABSTRACTQUERYREQUEST_H
#define MUSICABSTRACTQUERYREQUEST_H

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

#include "musicpagequeryrequest.h"

/*! @brief The class of the search result info item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicResultInfoItem
{
    QString m_songName;
    QString m_singerName;
    QString m_albumName;
    QString m_duration;
    QString m_type;

    MusicResultInfoItem()
        : m_songName(TTK_DEFAULT_STR),
          m_singerName(TTK_DEFAULT_STR),
          m_albumName(TTK_DEFAULT_STR),
          m_duration(TTK_DEFAULT_STR),
          m_type(TTK_DEFAULT_STR)
    {

    }
};

/*! @brief The class of the search result data item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicResultDataItem
{
    QString m_id;
    QString m_name;
    QString m_nickName;
    QString m_coverUrl;
    QString m_playCount;
    QString m_description;
    QString m_updateTime;
    QString m_tags;

    MusicResultDataItem()
        : m_id(TTK_DEFAULT_STR),
          m_name(TTK_DEFAULT_STR),
          m_nickName(TTK_DEFAULT_STR),
          m_coverUrl(TTK_DEFAULT_STR),
          m_playCount(TTK_DEFAULT_STR),
          m_description(TTK_DEFAULT_STR),
          m_updateTime(TTK_DEFAULT_STR),
          m_tags(TTK_DEFAULT_STR)
    {

    }

    inline bool isEmpty() const
    {
        return m_name == TTK_DEFAULT_STR || m_nickName == TTK_DEFAULT_STR || m_coverUrl == TTK_DEFAULT_STR || m_description == TTK_DEFAULT_STR || m_tags == TTK_DEFAULT_STR;
    }
};
TTK_DECLARE_LIST(MusicResultDataItem);

static constexpr const char *QUERY_WY_INTERFACE = "WangYi";
static constexpr const char *QUERY_KW_INTERFACE = "KuWo";
static constexpr const char *QUERY_KG_INTERFACE = "KuGou";


/*! @brief The class of the abstract query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractQueryRequest : public MusicPageQueryRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractQueryRequest)
public:
    enum class QueryServer
    {
        WY,     /*!< wangyi query server*/
        KW,     /*!< kuwo query server*/
        KG      /*!< kugou query server*/
    };

    enum class QueryType
    {
        Music,  /*!< query music*/
        Movie,  /*!< query movie*/
        Lrc,    /*!< query lrc*/
        Other   /*!< query external*/
    };

    enum class QueryMode
    {
        Normal,     /*!< query normal*/
        Meta,       /*!< query meta*/
        MetaItem    /*!< query meta item*/
    };

    /*!
     * Object constructor.
     */
    explicit MusicAbstractQueryRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by input data.
     * Subclass should implement this function.
     */
    virtual void startToSearch(const QString &value) = 0;
    /*!
     * Start to search single data from value.
     */
    virtual void startToSingleSearch(const QString &value);
    /*!
     * Start to download query result data.
     */
    virtual void startToQueryResult(TTK::MusicSongInformation *info, int bitrate);

    /*!
     * Set query mode.
     */
    inline void setQueryMode(QueryMode mode) { m_queryMode = mode; }
    /*!
     * Get query lite flag.
     */
    inline QueryMode queryMode() const { return m_queryMode; }
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
    inline void setSongInfoList(const TTK::MusicSongInformationList &songInfos) { m_songInfos = songInfos; }
    /*!
     * Get the current song container.
     */
    inline const TTK::MusicSongInformationList& songInfoList() const { return m_songInfos; }
    /*!
     * Check the current song container is empty.
     */
    inline bool isEmpty() const { return m_songInfos.isEmpty(); }
    /*!
     * Map query server string.
     */
    QString mapQueryServerString() const;

Q_SIGNALS:
    /*!
     * Clear all items before the new query start.
     */
    void clearItems();
    /*!
     * Create the current items by song name\ artist name and time.
     */
    void createSearchedItem(const MusicResultInfoItem &songItem);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    /*!
     * Find download file size.
     */
    bool findUrlFileSize(TTK::MusicSongProperty *prop, const QString &duration) const;
    /*!
     * Find download file size.
     */
    bool findUrlFileSize(TTK::MusicSongPropertyList *props, const QString &duration) const;

    QString m_queryValue;
    QString m_queryServer;
    QueryType m_queryType;
    QueryMode m_queryMode;
    TTK::MusicSongInformationList m_songInfos;

};

#endif // MUSICABSTRACTQUERYREQUEST_H
