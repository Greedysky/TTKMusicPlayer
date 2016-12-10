#ifndef MUSICDOWNLOADQUERYARTISTVIPTHREAD_H
#define MUSICDOWNLOADQUERYARTISTVIPTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_ARTIST_MULTI_VIP_WY = "NmZaR2FnQ0VKVzVEYmp4YVdadGwrSHQxT3IwSmJaRGFISmc5TnQrUEo4aTBFdFYvdGYyWW5odnFtWjhocSt3L1loVFFiZGM3S0I0REVhNUg=";    //wangyiMusic
const QString MUSIC_ARTIST_MULTI_VIP_QQ = "bFpoNnZ1cWFpUENVT3gxVDdxSlVheHMrRXZNWnVGWXJOdGM1dFJodUhlakhVVW9XRzhZWE51SjJ2M2xoNzdJN2I4bmNhRXVwbERlV0FUUmc=";    //qqMusic
const QString MUSIC_ARTIST_MULTI_VIP_XM = "aTROKzZXRmRSNWlnUWI3M0FEVVFWb3ljZzlQM1o3cUFTQjlvbFZtZ0IyaDhYLzZ2aUM1c2JSMmNGQjhvdWtESjBqQU1JVHgyK2xNUXRtWjI=";    //xiamiMusic
const QString MUSIC_ARTIST_MULTI_VIP_TT = "WXNQelBESDJyczJ5NGphYmg4LzhkcmdGUE5PM3V6bEhSYktDbTM2NkFXUWR3akdSOW5hdUEvaHdRVWVFUzREVnRGWEMrVnZlcU1vZnJDOXM=";    //ttpodMusic
const QString MUSIC_ARTIST_MULTI_VIP_BD = "WG90RTd1a1lmZXdaY0M1S3Baeldwb3BNQ2VzVzhHTnBtNFdQN1VGcVRxRk5TRlNGZmlzcVVPTkJnaE5IUG9VYUp0eGJMMGV5YjNPWHcxbmM=";    //baiduMusic
const QString MUSIC_ARTIST_MULTI_VIP_KW = "NUVhY0xHaU1ZVlI5Y1FyUTh3MW50enVhTHoxcHByMzVzR1dnbFp4eW5aanM2bG8wbGZVb2kvSkx1V2JJdTB5cG5kTXhVMGIvZ1hLamg0S0M=";    //kuwoMusic
const QString MUSIC_ARTIST_MULTI_VIP_KG = "TEtIR3Z6TFFZMzRhejNubUl4OW96TmRFV3E4SFFyVDZyRHhCZ3oydi8vRVVhTzNKMTVKMDFkR1JGSDVUUk1Fem1SSjNNUEovNDJQMHZYOUY=";    //kuhouMusic


/*! @brief The class to multiple query artist vip download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryArtistVipThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryArtistVipThread(QObject *parent = 0);
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

#endif // MUSICDOWNLOADQUERYARTISTVIPTHREAD_H
