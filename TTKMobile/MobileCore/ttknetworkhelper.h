#ifndef TTKNETWORKHELPER_H
#define TTKNETWORKHELPER_H

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

#include <QObject>
#include "musicobject.h"
#include "musicmobileglobaldefine.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class of the music network helper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKNetworkHelper : public QObject
{
    Q_OBJECT
public:
    enum Type {
        Null = 0,          ///*null*/
        SearcSong,         ///*search song*/
        SearcMovie,        ///*search mv*/
        DownloadSongIndex, ///*download index song*/
        DownloadMVIndex    ///*download index mv*/
    };

    explicit TTKNetworkHelper(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKNetworkHelper();

    //////////////////////////////////////////////////////
    Q_INVOKABLE void setQueryType(int type);
    /*!
     * Set network query type.
     */
    Q_INVOKABLE void searchSong(const QString &text);
    /*!
     * Search the song online by given name.
     */
    Q_INVOKABLE void setCurrentIndex(int index, const QVariant &data = QVariant());
    /*!
     * Set current play index.
     */
    Q_INVOKABLE QString getSearchedAttributes(int index);
    /*!
     * Get searched information attributes.
     */
    Q_INVOKABLE void setCurrentServer();
    /*!
     * Set current server index.
     */
    Q_INVOKABLE void setCurrentServer(int index);
    /*!
     * Set current server index.
     */
    Q_INVOKABLE int getCurrentServer() const;
    /*!
     * Get current server index.
     */
    Q_INVOKABLE QString getCurrentServerString() const;
    /*!
     * Get current server string.
     */

Q_SIGNALS:
    void networkConnectionStateChanged(bool state);
    /*!
     * Network connection state changed.
     * default status is true, means connected network.
     */
    void clearAllItems();
    /*!
     * Clear all items before the new query start.
     */
    void createSearchedItems(const QString &songname, const QString &artistname);
    /*!
     * Create the current items by song name\ artist name and time.
     */
    void queryDataHasFinished(bool empty);
    /*!
     * Query data just has finished.
     */
    void downLoadDataHasFinished();
    /*!
     * Download data just has finished.
     */

private Q_SLOTS:
    void downLoadDataChanged();
    /*!
     * Send download data from net.
     */
    void createSearchedItems(const MusicSearchedItem &songItem);
    /*!
     * Create the current items by song name\ artist name and time.
     */
    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    /*!
     * Update download percent\ total time and current time progress.
     */

protected:
    void closeWindowNotify();
    /*!
     * Close window notify.
     */
    void downForDownloadSong(int index, int bitrate);
    /*!
     * Download for download song.
     */

    Type m_queryType;
    MusicDownLoadQueryThreadAbstract *m_queryThread;

};

#endif // TTKNETWORKHELPER_H
