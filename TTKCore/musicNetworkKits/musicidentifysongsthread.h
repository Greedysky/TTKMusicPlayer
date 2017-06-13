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

typedef struct MUSIC_NETWORK_EXPORT MusicSongIdentify
{
    QString m_singerName;
    QString m_songName;
}MusicSongIdentify;
TTK_DECLARE_LISTS(MusicSongIdentify)

/*! @brief The class of the song identify query thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicIdentifySongsThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    explicit MusicIdentifySongsThread(QObject *parent = 0);

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void deleteAll() override;
    /*!
     * Release the network object.
     */
    bool getKey();
    /*!
     * Get query song id keys.
     */
    void startToDownload(const QString &path);
    /*!
     * To query song by input path.
     */
    const MusicSongIdentifys& getIdentifySongs() const;
    /*!
     * Get identify songs.
     */

Q_SIGNALS:
    void getKeyFinished();
    /*!
     * Get key data from net finished.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    void keyDownLoadFinished(const QByteArray &data);
    /*!
     * Download key data from net finished.
     */

protected:
    QByteArray hmacSha1(const QByteArray &data, const QByteArray &key);
    /*!
     * Encode the data into sha1.
     */

    MusicSongIdentifys m_songIdentifys;
    QString m_accessKey, m_accessSecret;

};

#endif // MUSICIDENTIFYSONGSTHREAD_H
