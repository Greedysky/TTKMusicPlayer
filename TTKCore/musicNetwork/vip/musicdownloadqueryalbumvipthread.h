#ifndef MUSICDOWNLOADQUERYALBUMVIPTHREAD_H
#define MUSICDOWNLOADQUERYALBUMVIPTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_ALBUM_MULTI_VIP_WY = "UjhkQ0JSaW5DUStFS1pLQ1VNSnJ4Z2l6R0hUM1hTMjkvQmFqYlg2bm56MGs5cnV3V2R3Z3BncUtRNEdJV2UvQ08yaEJ1Zz09";    //wangyiMusic
const QString MUSIC_ALBUM_MULTI_VIP_QQ = "dTArU0tFeEtqYVU4eHRFWm91L2poZEZFU281Z2hzblhDN2o0RnhCVjYxcElHZzdKZDdlelRNVkVJeHJvbUFHQXRkMUFCUT09";    //qqMusic
const QString MUSIC_ALBUM_MULTI_VIP_XM = "WkcyTzZ0b2tIZjNaRm1NbVNkMDZ3WlNRQ24xUTlYVEpIbnJ2WDQ5ZXl6WUJvZTRrU0p3Zm1qZ2RQeEVqQmtHd2QyLy9IQT09";    //xiamiMusic
const QString MUSIC_ALBUM_MULTI_VIP_TT = "STZ1UXVWSlFMOW5mNUNVc2hBYnlvdmlTWDJXUWRhVmFoYUdlTGcrMHFLT25RM0U0ajZVQmtXcXcwaHplS21GTTl2dHBGZz09";    //ttpodMusic
const QString MUSIC_ALBUM_MULTI_VIP_BD = "MC9QWW16ZU02U05KcDFVYnB6L3NwbC9ZZUcrU1Y1bTU2L2lkcDZPVStDTHNlK1g5Z0RkSUhXR3ZDSWxoNnBxb2p1K0V0UT09";    //baiduMusic
const QString MUSIC_ALBUM_MULTI_VIP_KW = "SkJoZWhOKzBVWkh5VnpnMm9vNWhmeDJJTWxYdDVySlpEejM3b1pGR04vZFU5RlR4VlpQK3dsS2pNcmdhblNOYjVXYnpIdz09";    //kuwoMusic
const QString MUSIC_ALBUM_MULTI_VIP_KG = "K1haejRhenVaVVMwbmFMMEh0bFdzajk4UkI1Mm1Hbm9OZ0d4UkRmWlZjdERtUEUrM1JwWkhQaVdIRDZNSXJaaVEydjBxdz09";    //kuhouMusic


/*! @brief The class to multiple query album vip download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryAlbumVipThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryAlbumVipThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchSong(QueryType type, const QString &text) override;
    /*!
     * Start to Search data from name and type.
     */
    void startSearchSong(const QString &album);
    /*!
     * Start to Search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    QString getCurrentURL() const;
    /*!
     * Get current url the user selected from mutiple queue containers.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation &info, const QString &size,
                                    int bit, const QString &url);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */

};

#endif // MUSICDOWNLOADQUERYALBUMVIPTHREAD_H
