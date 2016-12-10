#ifndef MUSICDOWNLOADQUERYARTISTTHREAD_H
#define MUSICDOWNLOADQUERYARTISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_ARTIST_MULTI_WY = "VkM4REtMY0VnYUxBK0NJMzJnaWtrQ2ZUdmN0TTE0ZFRlaEFmRlU2aGxVT2NDRjBGUnlwZTVIYkxkMGJINTV1N1BtTEpvR2Z5MWhyVFFQcExvOUxBem1CdjU0QlZSTkNBWG43bHRHMmtOVTMrWFRPMjhqNlNjdSs4Vmd2NVJBVTQ=";                //wangyiMusic
const QString MUSIC_ARTIST_MULTI_DX = "eGhCTGZIRjNTbWt1R1BIeUNqeFpQWmxKNE0rNDNMRlBORTMwVG9CU2pwM3dUdUNLTFJGQ2VJUk53aWt6TnNMVmUzNzhaM01mWmFXbDFlTHdZelkrRnJ6R1BiMk9YUUF2V1BFUGJDdWhYZ2d2ZVV2RGUvWW9WbkhKS2R6aHRjY1Q=";                //dianxinMusic
const QString MUSIC_ARTIST_MULTI_QQ = "bTEwMkw5ZWZkbDRqbnlwbkV5dTdLa202c2IzOTgwUUtnOTVId1orMm9BbmFPcGxnMEdWS0oxU0xTUVFTZDBGUFRDdmloOUpCZzB5cXh0SUdQUUtTWjlSazIvN0ZvWEY5czdKalEybHMrMjdPQWY0UmdQaXpNK2dzNmdBTmlqSnc=";                //qqMusic
const QString MUSIC_ARTIST_MULTI_XM = "RlZzYmNwc3lvOWFxVUZ4ZDFxSDArZlhnV0w1bVloUnBCNkJaRUJ5bHdzeUF6dkxhZnh4VmNmZlg2aDFHN0hqbHJwUnR5NmRaVXhKTVI2MUhLK2JtUWROeE5wZDNURHEycjVhWWJtVE9CTmV5ZCtEbUhhTnIrckphYnBmRGNZOU8=";                //xiamiMusic
const QString MUSIC_ARTIST_MULTI_TT = "VDNndWtoemZhZ1ptT1NBYmpZMTd5MytVZUw3ZEl1Tjk1RGhXTnBXUFpZRXNkeTMzUWN4Z3hkMFZxSFVWOU9kWUkyWllWbVZ0MXBLSVRRVWtpVENiTjh0K2VyTDd3WUI3WTlEUmlVODNsdEpEUjZ0aFdwOVNxaWhQMWxhVHdNQUs=";                //ttpodMusic
const QString MUSIC_ARTIST_MULTI_BD = "Q2YwcVNOM01pZVgycDFxcm9JUHlENHpET1RIOVBIVHV0NWlpTWVXYmVMdlRZUkhqWnM3Q3pvQ0hZY0tHRkNZUHYyZE1NM3c4cm5adVZEaFRiTklOVGEwL0xuVVlWNFlWYW5kQVRCb3VZcXFBTTJXTkI1bDdxZjhSK1ZUczl0bUk=";                //baiduMusic
const QString MUSIC_ARTIST_MULTI_KW = "TjZ1cWYvdllkNWZKaG80MlgvL1ViSDEwNGN6NDhmVk1VcEVIcU0yUHpqRzI3RFJ3OTRPRjJwd1dtWFZ0NktvTWcrenNkRE00Ymo0UG14YTVzd0dpZGs3RlBKdE5TcGltaC9jV3VyQzZnb2pBMG1QY1JicU1SQ3BnYTdLMiszSm4=";                //kuwoMusic
const QString MUSIC_ARTIST_MULTI_KG = "YTFzYXhqb2V5NzJyZndsU3hPQUczem9xa3BEMm0zWnc1WXU1L1BCK2RNVnM4TE1mY3RRaEdFVWJSSy84M1l5Z1k5dTdtb2FiZjNja3VLZHBVTGZPU0h3Y3kvTERpelByd3FmVHd3SGdNTmMwRWMvSVl6UXhMQWhjTUhvRkorRmU=";                //kuhouMusic
const QString MUSIC_ARTIST_MULTI_DM = "ZXZwc3pGZXE1OFBBNUtlZ0hjYWM4VHNNcWQ3ZGNTSlo1SVQzalFZYk15TmZQUmw4d2JCL0J6bEx0aXpTaXFhM2FMZHpHdi9Gdk1sTEZkeW5hMmU3MHVDOWVqREdDUFpNSXdIbU9QbXlqM1lMcHNMRmxmNTA0MzMrTnN4MG43bmg=";                //duomiMusic
const QString MUSIC_ARTIST_MULTI_MG = "YjNFNzdMS1A3OU9oeCsxaU1lWkR2T0M5UHEzcXF5YlV4RnZFSUorVGpwODJSZWEwSWZRZVBYU2F0aVFhRTdFRk95eGV3dTFpc01RMjE1QTFIaG1vb29iK1NwT3d6OGZkc3hGSndSc2cwOHh2emxnTFJZS21NZ2d0c0RWNHVpMDY=";                //miguMusic
const QString MUSIC_ARTIST_MULTI_MU = "L0ZZZ0paSENuODNTQ2xkaUZ4b1lDRTNrV0t3cEhncTloREhCcm00c2R3L3U5QnpuN0VBYUxRVHl0ZU8vZGYveU5MWjRrWGE1cmNZN3dRck1IOFlTajFJdHV1S3hIcG9UKzAzSWtKZnN5RUhMaEVDTlM1RkFuUjhtZjZpQUx6VkgyWmZ0RVE9PQ==";    //dingdongMusic
const QString MUSIC_ARTIST_MULTI_EC = "b0Vtekxqam9DMHJTVHA1OGh6MmFNUUtWYVdJNmtJblhCTTVSZWlxME1uWUMrcXhmOGlnKzdjMmgrUEx2VFBvU2grelNvNW8wVmxOaDQ2UlBhYzJFT01jMFYvUkIwWDVQUXpGOEcvaUZ4YVd2OC9JVlJTUUpTZTFWbEZ6dDZBV041ZFpQMVE9PQ==";    //echoMusic
const QString MUSIC_ARTIST_MULTI_YY = "SlFXTFM2eTBzQ285ajMwZVhwM0Fid09tL1NLaGFDbkFXVHBvY0doTzBIZ1BkU1BTRVE3YjJyUHdtZVduSCtYN3BZejI4VjZ4WlhhdG9iUWRqN1l3dlN2bk5GSWxUSGdhQ1Q0NFUzenhMUGZMc1d2eEJDMzhjZVRVR2dVMVY2TTE=";                //yinyuetaiMusic


/*! @brief The class to multiple query artist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryArtistThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryArtistThread(QObject *parent = 0);
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

#endif // MUSICDOWNLOADQUERYARTISTTHREAD_H
