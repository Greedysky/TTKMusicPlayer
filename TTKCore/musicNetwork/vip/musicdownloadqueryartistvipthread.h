#ifndef MUSICDOWNLOADQUERYARTISTVIPTHREAD_H
#define MUSICDOWNLOADQUERYARTISTVIPTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_ARTIST_MULTI_VIP_WY = "YlM0eHc3Z1hWSEpHRWRqTllUNS9Oa1RGR3g3UmhnRXhlb2FWUmN3RzhUdmt4eDlocmM2Wms2L3ZoR2h4SFE2cjJuZ2p3Vm1zWi9pN2dmNDJWVWt3enVFMEpENCtvellRa0JXZ2JGaFJpWmZ1SmhHaWJTMWpiYkViRHFudndDUmw=";    //wangyiMusic
const QString MUSIC_ARTIST_MULTI_VIP_QQ = "WjRrbDhydmFWazhIRnplZHZWY2g4Unh4SEtUSU0vN01zQXhGN0FDd3BoUGN5M3dkNXRzZEtsQ2FoVEUrc1JsaUtEOENkblpqMU9BRzJOSEtkYUViL2xIT0ZRT1UvT2llVDJXWXJ4NENqNGlKU0xGMGdabHBzRHV1akNtQlByVFU=";    //qqMusic
const QString MUSIC_ARTIST_MULTI_VIP_XM = "d09HZDFTZDJneXVPOFBkc0dLdmV1RGppN09oWHkzL01KTWcvS1FvUEc0M0VTUlNxMFdxRTdHMkYvTEJTWDVPblBwZ0ljcVN3QVNVQ1J2NlhlR2d3dHZRdC9PQmIxUnJLREtNZU5paVFxc1FITVBjWWFOVXpmTFM5U3BwbFd2Y0I=";    //xiamiMusic
const QString MUSIC_ARTIST_MULTI_VIP_TT = "dXg4OXB0aWhYRXJ6VHc1aUNxZFBuUTBPMjRqY01jRzJFeisyVG0ydDNZM3dEcDFDRzlXdlpJV2o3MCtpcHQrNDRzcWJwaFVpMStTcW40ZHJOT2dRbEZJQmNrcUw1cWtnbEE3N0ZFREN0blp2SWsxSTQ2L0FWQ2lxcmNXM2dLVlk=";    //ttpodMusic
const QString MUSIC_ARTIST_MULTI_VIP_BD = "Y3FSUzRGN3QzcXdKejJkL0JGYTQwM3RHOS9KRGRVR3RDZ001WUkyVGluSkkxT2VYL2p1Qjc2enMwdTE0d1huK0p2SlBndllUR2ZYYk9yOFB1SXRkaWlzUHAyWnZzbGJzVENTaGRuWHVmdUdESHpGajN1RlNnVTNNRmg3S2h5UUw=";    //baiduMusic
const QString MUSIC_ARTIST_MULTI_VIP_KW = "R2hCbzlKWFJ2SmM0c0o5VWJ6K005VzBFbE9uNWhBOHZWWk1hZWRzdGtsSkViaWhYenVZbmkzem5nZnR2bFVDUFJETUlmMHBLUytEYy9SSk5PWHBldTRSamdyTFdGRDlYeWVzL3NhUy84eWRZT1NrSTYrc3lMWldkV2YvNXIzdHQ=";    //kuwoMusic
const QString MUSIC_ARTIST_MULTI_VIP_KG = "ZkRNanIrM2o0eCtOUkw2ZVhVeGZHZ3FmdnVSa2h6eXVidWw5V051dmgzWUpjbmJnaVdTMnZDdVBCMEhMdkcyNWFXQzh2Zy9zaVBWNGIvK1NSRVBlSGxpTHBlWDN4NlZkditJWkQ5Q2cvYUxVSlZxdHRXeE1zZStuc2trT1I1ckc=";    //kuhouMusic


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

#endif // MUSICDOWNLOADQUERYARTISTVIPTHREAD_H
