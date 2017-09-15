#ifndef MUSICIDENTIFYSONGSTHREAD_H
#define MUSICIDENTIFYSONGSTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobal.h"
#include "musicnetworkabstract.h"

/*! @brief The class of the song identify data item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_NETWORK_EXPORT MusicSongIdentify
{
    QString m_singerName;
    QString m_songName;
}MusicSongIdentify;
MUSIC_DECLARE_LISTS(MusicSongIdentify)

/*! @brief The class of the song identify query thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicIdentifySongsThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicIdentifySongsThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;
    /*!
     * Get query song id keys.
     */
    bool getKey();
    /*!
     * To query song by input path.
     */
    void startToDownload(const QString &path);
    /*!
     * Get identify songs.
     */
    const MusicSongIdentifys& getIdentifySongs() const;

Q_SIGNALS:
    /*!
     * Get key data from net finished.
     */
    void getKeyFinished();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;
    /*!
     * Download key data from net finished.
     */
    void keyDownLoadFinished(const QByteArray &data);

protected:
    /*!
     * Encode the data into sha1.
     */
    QByteArray hmacSha1(const QByteArray &data, const QByteArray &key);

    MusicSongIdentifys m_songIdentifys;
    QString m_accessKey, m_accessSecret;

};

#endif // MUSICIDENTIFYSONGSTHREAD_H
