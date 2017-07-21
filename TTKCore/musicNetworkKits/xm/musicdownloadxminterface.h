#ifndef MUSICDOWNLOADXMINTERFACE_H
#define MUSICDOWNLOADXMINTERFACE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicglobaldefine.h"

///////////////////////////////////////////////////////////////////////////
const QString XM_COOKIE_URL          = "M1NCcStHeVRnSFkreGFUd2g2Wk9sY2tFQjNvbENLSm5PRDMzMkdCV2xQZE1RaTBZUEpPQkpZMEt5TGRhMmw4K2VzTXR1YXJJQ1NJbjdhOHNoTDYwQkRhQmZIdkI1c1d6d2xsSG1GODR3WitMQkZnNw==";
const QString XM_QUERY_DATA_URL      = "RUpCVXppVi9MbDllV283V25IUTYzaHRFYiswSit4OTBkaVVmMTlKTU5YYytkaThYYmdjc0hYQWNYajJ6cmYxK3hraW5jU05jRTRWc1dKQ1Y5TCtDTjQ1Y0VhOD0=";
const QString XM_QUERY_URL           = "VDRld0hsamNPZzFBYmVzRTdIUks3eW5DT28yZG1DbGlkSisyWFQxaTlTY3Zlb2pLYU9nWTBzMTJEenV0aGFUcDVvNzJSMTdpUm93SnFnM2oremhCWDlTc240Um5JR0ZJNko3bzF2Tjh6cThEWGdwT3BUeTRLTTE2N1VyN1NERlcvNUc5LzNaOGFudnREOTdBTmZMelVhc1ZBSjYvL01keFBOQW1xdz09";
///////////////////////////////////////////////////////////////////////////
const QString XM_LRC_DATA_URL        = "Vis5ZDJwQmFVSEN2RkR1VGRBaHNINTdnREFyd3dqZUM=";
const QString XM_LRC_URL             = "WUIweEdQZHRXSjhxVWo5VVd3ZjRDNkhib2hXdkw4bjBkRjJWUE1aZWZaLzF5emxpZ2xOeUx6V2tFbUEwdmpJTTl3SFEzZz09";
///////////////////////////////////////////////////////////////////////////
const QString XM_ALBUM_DATA_URL        = "WnRoTWJTU3IwZ1ZIa05YcEJKTWF2Um53cVFHSDJRdUdnaTdicXc9PQ==";
const QString XM_ALBUM_URL             = "aUxaMFJ6b3ZldFBvTXNpcFhOTmNvMmk4TXBlODNueHo0RWphSDh6Q2tBN3B6WXM3b0ZqZzN3Q1MvUzBUUXFud0NlU1Yxdz09";
///////////////////////////////////////////////////////////////////////////
const QString XM_SONG_DATA_URL         = "TWl1SGl6RzUzUndpNFlQTFdEWVdxQ0I4S21SZUtKOVd2RFRvQ3BXTGlubnFwcWRhazVabG1zdnI4MDNranY2OEd4ck94UGZpVVg4K3VOYi9UdHZCTGVZek1uTTdSY0pw";
const QString XM_SONG_URL              = "MDFXcWwxdmVuaVVqc0hQOG9BMDZpQ01ReGx4b0N0d3BsVkZEcEg4cnhaU0N0a1ZDVkh3L0hVUnFhazBhbUZqMGYwOFVoUT09";
///////////////////////////////////////////////////////////////////////////
const QString XM_ARTIST_DATA_URL       = "aEorVXFna1ZnaWdUUkF1ZVRaYi9vQ1JhcjZSU2RGaHg3UXRZdExIQk85d3BtY28wK2hKL2l0NkViZWhwaUhoUjB1Z0o3clZSRXl4K0dFaFVUci9GaDlQV3hhRitBeUpSSFUyZ2hRPT0=";
const QString XM_ARTIST_URL            = "aDJ2V2JmUWhPK2Y2VnhsSDBXT2ZGQkxuUDBHUFo4MWhWWDFIT1JRSWdjM3M1UVY2YWIvc0xBbDN1V2hlTXFGMDFFRTRTUT09";
///////////////////////////////////////////////////////////////////////////
const QString XM_MV_ATTR_URL           = "NDY3SUJhdXhRUHA4bUVMUmdXSGYyeXpTcmkzcngwYXpRMFZ2UlE9PQ==";
///////////////////////////////////////////////////////////////////////////
const QString XM_SG_COMMIT_DATA_URL    = "bWJNaVZaUC9UQkRudjZYTCt3QjIxR1hrck9UN1FDWWFjbm5LN1RzanV6Q3NqREtmZ0NQL0RFZWYzR0hNZDZ4aW1KMGU1bzVBaWRJT0kzb3BTaWhKTjhlZllWV3E2OEZkT0hYS2pqSHkwSXU5QmRsUjVPdmdzUCtWNi9nY3BhK3c=";
const QString XM_SG_COMMIT_URL         = "dEh6MVNYd2dSdDFnR2NyREl0RittVWxwQVh5UkpTeUpwRGRsallKKzBoaEV4cm9JTnRnYStZZTN0ZFQvTjVYRUlyK1JqOE1SMVhjPQ==";
///////////////////////////////////////////////////////////////////////////
const QString XM_SONG_TOPLIST_DATA_URL = "dUhvTkxueHI0V1h1QXFvNGhPbHRiUE5xZWp4aGo4dW8vZU9OU0lBR094YU50dFdaSmVBNE5oditlWDA9";
const QString XM_SONG_TOPLIST_URL      = "QkhwMmRoR29zNSszUGdoR09Yb0NNSFRZV0lkYUNQVVArV3piRzlWMWw2RnlHc2IvMURpeFJPaXhqaHNWUlh3UVBjN3JkRm1PRjFFcE9ISGY=";
///////////////////////////////////////////////////////////////////////////
const QString XM_PLAYLIST_DATA_URL     = "eGFxWVE3OFQzNUErdkZ2VmJmZWNGRm5MVGQ5OG54ZC9oWW96akFIK0tXWU1ML1lKUktEanMrNE11N3BQYkVNYTIxbTI3Zz09";
const QString XM_PLAYLIST_URL          = "dU0zeENFRlhjYkRnd0QvWmg4RTJjSkZqNEo5SFYwK0pwSXZKWG1PcWdWbGVLUUlVUW0yTkM2ZkdSRXEzemxNelZiMkdGZmF4eFFJPQ==";
const QString XM_PLAYLIST_A_DATA_URL   = "ZVlSN3dmUmw4UzNrbVduZGdITVNxRm5oZFBjaGxpWWFVQzVyT1dQRExya29OcGt5bW15MnlQRko3M09CS09qSThvUC80OXN0Q1JtUTZXZDcxZGNlQVhDNzBocysyVWFxQjVEV20zbElvTERHcGpRTnM4SmZOejYwM3BiZlRZV3M=";
const QString XM_PLAYLIST_A_URL        = "ZVpnUGt4NWZnUDRGQ0doWU1POEl5RDdWc0dpVlNtMWFzNlJFQldKVHFydUd2VGpqRDRkbERYcVFlY2N0MUk2MERvTUh2Y3FPNStIUXhYUHI=";
///////////////////////////////////////////////////////////////////////////

class QNetworkRequest;
class QNetworkAccessManager;

/*! @brief The class to xiami query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadXMInterface
{
public:
    void makeTokenQueryUrl(QNetworkRequest *request, const QString &query, const QString &type);
    /*!
     * Make token query url string.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariant &key, const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongLrc(MusicObject::MusicSongInfomation *info, const QString &songID);
    /*!
     * Read tags(lrc) from query results.
     */
    int map2NormalBitrate(const QString &bitrate);
    /*!
     * Map json bitrate to normal bitrate.
     */

};

#endif // MUSICDOWNLOADXMINTERFACE_H
