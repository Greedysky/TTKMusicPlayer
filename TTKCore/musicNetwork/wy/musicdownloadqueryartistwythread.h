#ifndef MUSICDOWNLOADQUERYARTISTWYTHREAD_H
#define MUSICDOWNLOADQUERYARTISTWYTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

const QString WY_SONG_ARTIST_URL    = "VURUWGM0c0NHamdldHFWcGwvZWlxS24rZ3Fpc1o0TEZmUDRDUDl3aWQvdklQN0VCeVpicmxRPT0=";

/*! @brief The class to wangyi query artist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryArtistWYThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryArtistWYThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchSong(QueryType type, const QString &artist) override;
    /*!
     * Start to Search data from name and type.
     */
    void startSearchSong(const QString &artist);
    /*!
     * Start to Search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    QString encryptedId(qlonglong id);
    /*!
     * Encrypted music info id.
     */
    QString encryptedId(const QString &string);
    /*!
     * Encrypted music info id.
     */

};

#endif // MUSICDOWNLOADQUERYARTISTWYTHREAD_H
