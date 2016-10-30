#ifndef MUSICDOWNLOADQUERYMULTIPLEVIPTHREAD_H
#define MUSICDOWNLOADQUERYMULTIPLEVIPTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_REQUERY_MULTI_VIP_WY = "TjJtZUlFWDc4Z0o1MHo1RnZHWi9reDBpWGFJZEZudWx6Qjc4NHNkTFR0aEtyZWllVEROdkk3UzNldWd6MlZrVTAwOVZtVjd2UFg1T3JkWXpPNlNRQXBNaHpyNGJHbjN4aHZCUXBhRWYzR2orZ3RneWw0MGRrWUtyQkpLQ1llVnM=";    //wangyiMusic
const QString MUSIC_REQUERY_MULTI_VIP_QQ = "WWhHS1BvdnM5NGRxUWVBdUUxbXdFUXAvTkZuRDdlY2dBOSs3MzIwSTdxYi8zeVVJQjRUQ1lKR0hVNFRPbmRsRHZFYlRSOEduVkpuZjRZZkRDY0ppbjJTY3l5Q2g0dUhrUEtqYnl2cEJiN3hFbC9wclhyUTByWWc0Y09LNzlwajI=";    //qqMusic
const QString MUSIC_REQUERY_MULTI_VIP_XM = "STdpUzJOQTF4NlRCZnVnVFlJYnhHa1kyUWhOZit2d2NSWWFwN2ppanltdGxpU01Td2RwZ3NFQjExM3JLOFlZY28vVndkeUkzeUIyVEpVTytwekw5SnFqUmQ0R0ZBVVBJNG9HRVRqeUExeEdJeldYRk1FMHBWZHVCZmx4ai9DeUU=";    //xiamiMusic
const QString MUSIC_REQUERY_MULTI_VIP_TT = "LzNxRm5XYnY1OHROVkVvQTJubHVBVmhzQUZ2bzNOb1dpSFVHeDFOMmlNVk5rYiszYlpzZXVmVWJ5NmhQcE9SRER6eERhMjM0VmI1eW5ieFVYb1lPc1krUXNXT3VvamUyVytGbmRQTkJuV3J5aWY3azdQSDZKK0dBSEl0Q2FGblY=";    //ttpodMusic
const QString MUSIC_REQUERY_MULTI_VIP_BD = "VGRsSHkvMElHV0c4aVNRSGlNL3JpYXRSODdwYkpMdGwxSE50azZYZ2w1aS92cDliOTZCL1BsRllVbTlBRDdadmhaaTdBMlhidHBmT1luSTVabGhuQ0ZjOERIanQvaGhsZ01CVEVTaHhiOFBJZDR5cURGZXc1cUx2clUxTzhkNU0=";    //baiduMusic
const QString MUSIC_REQUERY_MULTI_VIP_KW = "bERma3U1OStIUGpHblBNMVk1OUZkekVSYXdlSHZCZlVocDZneU9aR1UwM0NrM2ZMc0hxQkRnUUJSdThwRnlQeTVGMzlwVGxkWmNJcURXd0V5WHJvSlRtckQ5cTZYM3RTL3Q2eElacThCUy9WMGhSVXJkQnJoa0czL1pwWlZQbkg=";    //kuwoMusic
const QString MUSIC_REQUERY_MULTI_VIP_KG = "a0lOZDVWNCtHM1Znc25ZU1ZiWHVwTjFuNGVxT2tUQzlJYzVuQ1VJcDVYRkFiU1pkVnpWVU5EeTR5dmdqK2NDUzZtWCs5NXR4eXE2YU1pN2dOKzJqSEw3TC8wWXJYSW5MWHJNd1FsdWlWalIrWllCM2NwWGxGdGYvSUJ0S29YRTk=";    //kuhouMusic


/*! @brief The class to vip multiple query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryMultipleVipThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryMultipleVipThread(QObject *parent = 0);
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

#endif // MUSICDOWNLOADQUERYMULTIPLEVIPTHREAD_H
