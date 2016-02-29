#ifndef MUSICDOWNLOADQUERYTHREADABSTRACT_H
#define MUSICDOWNLOADQUERYTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include <QNetworkReply>
#include "musicglobaldefine.h"
#include "musicobject.h"

class QNetworkAccessManager;

/*! @brief The class to abstract query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryThreadAbstract : public QObject
{
    Q_OBJECT
public:
    enum QueryType
    {
        MusicQuery, ///*query music*/
        MovieQuery, ///*query movie*/
        LrcQuery    ///*query lrc*/
    };

    explicit MusicDownLoadQueryThreadAbstract(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicDownLoadQueryThreadAbstract();

    void deleteAll();
    /*!
     * Release the network object.
     */
    virtual void startSearchSong(QueryType type, const QString &text) = 0;
    /*!
     * Start to search data from name and type.
     * Subclass should implement this function.
     */
    void setSearchQuality(const QString &qual) { m_searchQuality = qual;}
    /*!
     * Set search data quality.
     */
    void setQueryAllRecords(bool state) { m_queryAllRecords = state;}
    /*!
     * Set wheather query all quality of records.
     */
    inline int getSongIdIndex() const { return m_musicSongInfos.size() + 1;}
    /*!
     * Return the current song container size.
     */
    inline const MusicSongInfomations& getMusicSongInfos(){ return m_musicSongInfos;}
    /*!
     * Return the current song container.
     */

Q_SIGNALS:
    void resolvedSuccess();
    /*!
     * Query data finished.
     */
    void clearAllItems();
    /*!
     * Clear all items before the new query start.
     */
    void createSearchedItems(const QString &songname,
                             const QString &artistname, const QString &time);
    /*!
     * Create the current items by song name\ artist name and time.
     */

public Q_SLOTS:
    virtual void searchFinshed() = 0;
    /*!
     * Download data from net finished.
     * Subclass should implement this function.
     */
    void replyError(QNetworkReply::NetworkError error);
    /*!
     * Download reply error.
     */

protected:
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    MusicSongInfomations m_musicSongInfos;
    QString m_searchText, m_searchQuality;
    QueryType m_currentType;
    bool m_queryAllRecords;

};

#endif // MUSICDOWNLOADQUERYTHREADABSTRACT_H
