#ifndef MUSICDOWNLOADQUERYWYTHREAD_H
#define MUSICDOWNLOADQUERYWYTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

const QString WY_SEARCH_URL        = "SFIwVWhHbEpXckhQT0VZUG5tUnBaYUNva2lUZXhSQnNuSkNBOGFMYlBhL2Z2YllBNXBWajFnPT0=";
const QString WY_SEARCH_QUERY_URL  = "UUs1NUMwUk9yeExmUFVCakUrS29NNm5OQ05lYUV3ZzJPS3NLVW9lZXhNaGwrd2Za";
const QString WY_SONG_URL          = "SkRzeTdTcHMvUVpaa01hcWdveklVK2FHbXRlRmRiTXYvYzRHdS9Hc01ZMllBaFlKNlp4RFBodHV0eERPd2haMEM3MjAzQT09";
const QString WY_SONG_MV_URL       = "cmVKem5MSmhzNk1KeUFRcXJFU2xrcHU3cnhwWHo2TVBNOFIwOC8rSldoSThUbmhQRVd0NEhTSnZGMytkVlFENUZTZ1lDekFLUkJvPQ==";
const QString WY_SONG_PATH_URL     = "bHp1OWliOUVIRS9sOWIvTDROc2ltMXJyaElUS1FUNS9kamhPUDB1Y2JPS3BqU1lJL3BiVlRBPT0=";
const QString WY_SONG_PIC_URL      = "eEZpOHNEanlmUmxITG9PRHI4aElxcDRqY1F5VjE1SmtMYm9lV3Z0eVpuakpBWFV4aFNZbDF3PT0=";
const QString WY_SONG_LRC_URL      = "TjNjL2ttNVR4cGluaEpwU1B2VzNucFhEWUdkRERkempFcThjWEIvMzMzMzVjaWVoR3ZNYXN5MHZsOE9sMTQ1TG1yeDNVQT09";

const QByteArray WY_BASE_URL       = "MVNVTXo4bW9WdHhXR1dXeTZmU3k5dmFOcGlua1VOMlE=";
const QByteArray WY_ENCRYPT_STRING = "3go8&$8*3*3h0k(2)2";

/*! @brief The class to wangyi query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryWYThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryWYThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startSearchSong(QueryType type, const QString &text) override;
    /*!
     * Start to search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    void songListFinished();
    /*!
     * Query song list finished.
     */
    void mvListFinished();
    /*!
     * Query mv list finished.
     */

protected:
    void startSongListQuery();
    /*!
     * Start to search song list by id.
     */
    void startMVListQuery();
    /*!
     * Start to search mv list by id.
     */
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

    QSet<QString> m_songIds;
    int m_index;

};

#endif // MUSICDOWNLOADQUERYWYTHREAD_H
