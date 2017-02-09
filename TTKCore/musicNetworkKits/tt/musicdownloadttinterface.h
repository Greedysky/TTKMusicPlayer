#ifndef MUSICDOWNLOADTTINTERFACE_H
#define MUSICDOWNLOADTTINTERFACE_H

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
const QString TT_ALBUM_URL       = "bEc2U0x6dUkxSUE3MkJMdzBUN1l2aFowMDFGUFV1Y2F3MVA3SDdDdzlIY2JxMCtsZ3BXZkpGSWhSY1JsanVGM2F4NldrQlQrRytkMXphT2JmNnIxWEE9PQ==";
///////////////////////////////////////////////////////////////////////////
const QString TT_SONG_SEARCH_URL = "c05JdGEwSjVrV1VGMEJzd0kvSzFlenlSS2hESUFuVHFYMVB1Z3dFTGlsUlAwVVJTSC9ZQTBhUkNXOWNueDZvME9vWlE1OVhtcnhYd2NSa1BZZHJNU1E9PQ==";
const QString TT_SONG_RECOMM_URL = "dHR1Vm84aHcwandxV21DSXA4UlpyTlJ1MnROcVptNG8rbFV2TStCN3hQeWtaeFhiZUxvREN2VmJSS2l1aGljcW11Mm5EQnhqVWU5T2tQUGhoSjhnV1E9PQ==";
const QString TT_SONG_LRC_URL    = "eDlRVnBMS1RYRXBvM0tBMUxaRVlsa0dwcEpzY25yMVRSalJHWndjV2p5MmdOSEhRbU5nYzBsU0gvb291R2QxRzNXUS9DbnhxVllOY1ZhZVZDa3pOMVNCYnVhdHJBTWxlaC9xbWRBPT0=";
const QString TT_SONG_PIC_URL    = "cUVYKzBOekUxTDdDcjZ1VWVhc2E1azcyUTJNQnpHUXlOclZ1RmY4THBXc0VGdjVsdXVqYTNabEtYSVBOSkhYLw==";
///////////////////////////////////////////////////////////////////////////
const QString TT_ARTIST_URL      = "aFozbkdtREt1Z3VLaU9MS2hEVDlxMUpCU1cxOExpZUpOelo4QmpMakdwWWxaTGIyWVlpMGpubU11QmgxNE5IaEUrbVJjazZ2TzVJVHRYeGNXaGx0cXc9PQ==";
///////////////////////////////////////////////////////////////////////////

/*! @brief The class to ttpod query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadTTInterface
{
public:
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    int map2NormalBitrate(int bitrate);
    /*!
     * Map json bitrate to normal bitrate.
     */

};

#endif // MUSICDOWNLOADTTINTERFACE_H
