#ifndef TTKNETWORKHELPER_H
#define TTKNETWORKHELPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicobject.h"
#include "musicmobileglobaldefine.h"

class MusicDownLoadQueryMultipleThread;

/*! @brief The class of the music network helper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKNetworkHelper : public QObject
{
    Q_OBJECT
public:
    enum Type {
        T_Null,              ///*null*/
        T_SearcSong,         ///*search song*/
        T_SearcMovie,        ///*search mv*/
        T_SearcLrc,          ///*search lrc*/
        T_DownloadSong,      ///*download song*/
        T_DownloadSongIndex, ///*download index song*/
        T_DownloadMVIndex    ///*download index mv*/
    };

    explicit TTKNetworkHelper(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKNetworkHelper();

    Q_INVOKABLE void setQueryType(int type);
    /*!
     * Set network query type.
     */
    Q_INVOKABLE void searchSong(const QString &text);
    /*!
     * Search the song online by given name.
     */
    Q_INVOKABLE void searchMovie(const QString &text);
    /*!
     * Search the mv online by given name.
     */
    Q_INVOKABLE void searchLrc(const QString &text);
    /*!
     * Search the lrc online by given name.
     */
    Q_INVOKABLE void downloadSong(const QString &text);
    /*!
     * Download the song online by given name.
     */
    Q_INVOKABLE void setCurrentIndex(int index, const QVariant &data = QVariant());
    /*!
     * Set current play index.
     */
    Q_INVOKABLE QString getSearchedAttributes(int index);
    /*!
     * Get searched information attributes.
     */

Q_SIGNALS:
    void clearAllItems();
    /*!
     * Clear all items before the new query start.
     */
    void createSearchedItems(const QString &songname, const QString &artistname);
    /*!
     * Create the current items by song name\ artist name and time.
     */
    void createDownloadSongQuality(int bitrate);
    /*!
     * Create searched downlaod song by bitrate.
     */
    void downForSearchSongFinished(const QString &key, const QString &path);
    /*!
     * Download (cached) song finished.
     */
    void downForSearchMovieFinished(const QString &url);
    /*!
     * Download (search) movie finished.
     */
    void downForSearchLrcFinished(const QString &path);
    /*!
     * Download (search) lrc finished.
     */
    void downForDownloadSongFinished(const QString &path);
    /*!
     * Download (download) song finished.
     */
    void downForDownloadMovieFinished(const QString &path);
    /*!
     * Download (download) movie finished.
     */

private Q_SLOTS:
    void downLoadDataChanged();
    /*!
     * Send download data from net.
     */
    void searchDataDwonloadFinished();
    /*!
     * Search data dwonload finished.
     */

protected:
    void dataForDownloadSong();
    /*!
     * Query for download song.
     */

    void downForSearchSong(int index);
    /*!
     * Download for search song.
     */
    void downForSearchMovie(int index);
    /*!
     * Download for search movie.
     */
    void downForSearchLrc(int index);
    /*!
     * Download for search lrc.
     */
    void downForDownloadSong(int bitrate);
    /*!
     * Download for download song.
     */
    void downForDownloadSong(int index, int bitrate);
    /*!
     * Download for download song.
     */
    void downForDownloadMovie(int index, int bitrate);
    /*!
     * Download for download movie.
     */

    int m_currentIndex;
    Type m_queryType;
    MusicDownLoadQueryMultipleThread *m_queryThread;

};

#endif // TTKNETWORKHELPER_H
