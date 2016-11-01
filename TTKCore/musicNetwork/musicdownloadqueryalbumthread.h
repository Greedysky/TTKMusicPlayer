#ifndef MUSICDOWNLOADQUERYALBUMTHREAD_H
#define MUSICDOWNLOADQUERYALBUMTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_ALBUM_MULTI_WY = "VGRNSDZsKzBHSjhUd2lFR3BqQVMrUmJVdEdHaS80a1JSSXVuRGxyZmg3YjNmQ05QMEhZRGh6REJnYkxPQWptZ212dm96bFl3L1duakQwdTJhR3dEVFNIdlk0UlNUT1NmT0RPNmRMc3ZUelh6SnZ2dkhFdXZ5cFA0cFJlMVg1Y1Y=";    //wangyiMusic
const QString MUSIC_ALBUM_MULTI_DX = "Zzc4WkFrdmhNcFhIcndUN1UyaXlOVW5USHB6djZDaGptSjM3aGk3OTFlRUdNRHQ5V1l3K3c2VVVEdlhkYXRlN0NMMUFsRGg0M3lMci81eVdacWt2UnV4emdrRkxoWU1FY1JYUCtwa3R6aVdRN3FPQUtoTVZ1K3lhSW1tQ2FZT2g=";    //dianxinMusic
const QString MUSIC_ALBUM_MULTI_QQ = "WUM3QmFFSXFlMXJQQVFqbWtpUXpHTXp6VnJhTW8vMFREMlpqdkhlUWxiVTRtOG12NDJYS09LdFRkMlR2ak95d0tqaU9MaytwZ2F6WXdSQWFrZnYrZHl4SDBqTmpadXE2K3RtdHpXTjROdE1HRkdtUHF1VkR6ZHhmV05PMGZmeHQ=";    //qqMusic
const QString MUSIC_ALBUM_MULTI_XM = "cjlJc0cxbmY0ZGtCM0QxbG9Eb3oyN204NVBRRVVNczAyNVloUUxkNXdFUEQ1cEV1cjJRbE4yblg0dmkzTWIxOTRpQVl2T09xTkYydUlzWWdRRTA0VWVoMVNlQmQ3d1JNSFovamZDL2dJaFBSaGQrK29uQ3dqY0lzU3Q5aEQwQk8=";    //xiamiMusic
const QString MUSIC_ALBUM_MULTI_TT = "TThIYUNNQjBNNmgvUGNRZEpMTkw2MnlEVkJOWnFEWFVFWk9EMkdmb0VaREpMdlNZLzcyWUZ5QUJmRlJRcGlTQjZiT3BaS3d2UU9NQUlrRlFIbmJuNTZWU0xFeFFMb3NuVGdUdzhOZTI5bHRsQ1EvL1NRdlQ2cXRUbHl0Qk1wRUY=";    //ttpodMusic
const QString MUSIC_ALBUM_MULTI_BD = "NGFyWlFjdDd5YjdXNSt1eldFL1VNcjZrMW1aUzcrN0NZbWN4UHFiVEpBRmRjSFdNRHdPOWlTcXU0MFlGUXBFQ3hwUmlsUDVITVBHcDhEVllST204WXhpMk5JcEpyNzBlME45Mm04bVp3OVZMOW9WTWIyQUhFNHJnejFQOHZEbnk=";    //baiduMusic
const QString MUSIC_ALBUM_MULTI_KW = "dkNXcCtDVXZBaGp5WnlaL010a1J3M1FsUFFjdFFTMC9lSUU4Qmg3VDcxaFk4djk3czBVYnNvWlgxL2p5a25KLys4OXNRM0VJWllKM3RCYUN0SzA1VjQ0UWNMZExmemFpU2NkQ3NDSWJMVFMreEhkSEluM0p6Tk1hNk5MSDBTc2E=";    //kuwoMusic
const QString MUSIC_ALBUM_MULTI_KG = "LzhOdVAzb1JLbGI4bUdaZHdjZWorRW5aYTRqcEZZeVFGRk5NSjF0OFlNL1dGaENhTG1UbFV0Vm1DY3NaY24yTC8rRXlFRGlWeWNJaklzdnFKbnp5cGRJdnFIQnpsc2s1SURscFZLY1dtNG12ZjdjMGk2azZ5M28yNE1KQlNwK2c=";    //kuhouMusic
const QString MUSIC_ALBUM_MULTI_DM = "MVFIcHgyZ3o3Tmh3ZXhKNWpUY3JTVTU2WjVFMmRKODNkcGpPK0c4aUxQanBSSjd1NkQ2ZVJETVlxR0t3M2QwVnVmMlFZZVJpN3duZnlHb3ZoRlBhei9Jc2Z3eDFFYWRCU2tJbDhTRTYxOFppaGs1a0gwc1R5MnFtbE5uT1JMbFk=";    //duomiMusic
const QString MUSIC_ALBUM_MULTI_MG = "YWQ0Sm4vbFRxaXEvL0tHMlN3MjBUMjBCcUFOaGx5Q1loMllmMFppMjRTMm16T3AxSzZzeldZUi9pUTFzTmJZZHNvZFhJcXR5MWl5a3RTczNEVXNOUjlERmFuY2RDRE4xRGI0N20vdGN3c1dxQXh0YjVVanYxN2hxY0FNeDVRNVE=";    //miguMusic
const QString MUSIC_ALBUM_MULTI_MU = "MnlZTFlOVjRuYjMrV28vWFREWjNmRzZIbWI0RjA4WmhldFY2RTdud29sMXFoeEN4SnJhV0tROEhUd216SmszVElxZEZoSUVEOXNaUXM5TUdWSVNZYWdFeFNqODNBOGV2K2RhVG5GNFc0TlJwSks5M3BRZlRlRjJhVThCdFFUdi8=";    //dingdongMusic
const QString MUSIC_ALBUM_MULTI_EC = "Z3JXM1pvWnBXcUU3K0d4U3Qzc3RpcDU2UmFTQlMyUUhBNUpHQnc2Q0VEbFFiSWNYb1VCQ0FQcUNjbWtlY3oweGVHK3hya0Jsd0Z1VGlBekw0LzVhM0JaSzJFWlJwYmJZSjAxK1ZQTktzb2hYblpuRmlDN1VndEl6R0t5SXZROEI=";    //echoMusic
const QString MUSIC_ALBUM_MULTI_YY = "UCtRQmU1c2hoZmp3akpXOU9uczRIeStseE1JVXVHWXNOV1d6VkRkUm4vSVNoTWFLOUpvN3JrdDhESmhLc1BxTjFMM2dIbFQ3TmZWTlpySEdCbHdLLzRsbXhYdXpYYWsvZ1piQXdtUlg5aSs3UE85Qjk2OFoweE80cXdMdGxBUk4=";    //yinyuetaiMusic


/*! @brief The class to multiple query album download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryAlbumThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryAlbumThread(QObject *parent = 0);
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

#endif // MUSICDOWNLOADQUERYALBUMTHREAD_H
