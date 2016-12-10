#ifndef MUSICDOWNLOADQUERYTTTHREAD_H
#define MUSICDOWNLOADQUERYTTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

const QString TT_SEARCH_URL     = "c05JdGEwSjVrV1VGMEJzd0kvSzFlenlSS2hESUFuVHFYMVB1Z3dFTGlsUlAwVVJTSC9ZQTBhUkNXOWNueDZvME9vWlE1OVhtcnhYd2NSa1BZZHJNU1E9PQ==";
const QString TT_ALBUM_URL      = "RWZvMi9WT2w4dDZkTER2ZExJRGNtUDROWGRlK05sd2FwNWtVVG9UcHlpRXUxUGNFTitlZzFsdzl3bVl3YmFLSnRjZ3NheElSeDRFTDNJQjRCbExaZGc9PQ==";
const QString TT_RECOMM_URL     = "dHR1Vm84aHcwandxV21DSXA4UlpyTlJ1MnROcVptNG8rbFV2TStCN3hQeWtaeFhiZUxvREN2VmJSS2l1aGljcW11Mm5EQnhqVWU5T2tQUGhoSjhnV1E9PQ==";
const QString TT_SONG_LRC_URL   = "eDlRVnBMS1RYRXBvM0tBMUxaRVlsa0dwcEpzY25yMVRSalJHWndjV2p5MmdOSEhRbU5nYzBsU0gvb291R2QxRzNXUS9DbnhxVllOY1ZhZVZDa3pOMVNCYnVhdHJBTWxlaC9xbWRBPT0=";
const QString TT_SONG_PIC_URL   = "cUVYKzBOekUxTDdDcjZ1VWVhc2E1azcyUTJNQnpHUXlOclZ1RmY4THBXc0VGdjVsdXVqYTNabEtYSVBOSkhYLw==";

/*! @brief The class to ttpod query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryTTThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryTTThread(QObject *parent = 0);
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

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

};

#endif // MUSICDOWNLOADQUERYTTTHREAD_H
