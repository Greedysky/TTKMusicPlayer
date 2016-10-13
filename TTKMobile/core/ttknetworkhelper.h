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
        T_Null,             ///*null*/
        T_SearcSong,        ///*searc song*/
        T_DownloadSong      ///*download song*/
    };

    explicit TTKNetworkHelper(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKNetworkHelper();

    Q_INVOKABLE void searchSong(const QString &text);
    /*!
     * Search the song online by given name.
     */
    Q_INVOKABLE void downloadSong(const QString &text);
    /*!
     * Download the song online by given name.
     */
    Q_INVOKABLE void setCurrentIndex(int index, const QVariant &data = QVariant());
    /*!
     * Set current play index.
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

    void downloadFinished(const QString &key, const QString &path);
    /*!
     * Download (cached) song finished.
     */

public slots:
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

    void downForSearchSong(int index);
    void downForDownloadSong(int bitrate);

    int m_currentIndex;
    Type m_queryType;
    MusicDownLoadQueryMultipleThread *m_queryThread;

};

#endif // TTKNETWORKHELPER_H
