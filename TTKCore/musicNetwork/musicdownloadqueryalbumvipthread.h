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
const QString MUSIC_ALBUM_MULTI_VIP_WY = "eDJZVzNQbEJEWG5SSmpCK2d5UlN5ZlFvVGlkZW85SnlQbUs3NnJ5RmtCZndMYUdzTWpyd1pYdlYrcjZaaFc0eXVGQXgxclpFaW9qaDZObGc5MjhQekp1VWk1dE5xL3dvUTA0ajBGNFl4WVF1TGplc1lNK2tnUT09";    //wangyiMusic
const QString MUSIC_ALBUM_MULTI_VIP_QQ = "SDVIY2ZOU3o2bTdMZXJNd0Z4UmxTV0hQYzdWVjU3VFdBMlo1SzhMWTVBY0diTFpKMGhhdWh1WXVJY3hFMTJORW9lTG9WR05iWDQzZUlHOE42MSsvTTV3YllxTTZqdXJLZmxDSEZSWG1hZldpTFZCNlFxQVBidz09";    //qqMusic
const QString MUSIC_ALBUM_MULTI_VIP_XM = "LzhtcWRJZ1NIdkJGQUE2OHhiYk52K3dGR0piNUluQzNhZWlKc2tIbEdIWU5iM0NZa24zNW1vVWlaSnFvWUdYOWlNaXdOa0EwdWQ3RlVmclkzMFBuZjRIclVMZFVadWpSUlBOSzgrZ0hxVERTZ01iZWlsMGR0Zz09";    //xiamiMusic
const QString MUSIC_ALBUM_MULTI_VIP_TT = "Y3IyUlZ4WC9oSTV5SkM0Ym5lV0xTeVZjSTRnTXYrKzNXU2RlRlJpc2ZibWRkQnV4Z2ZuSTZPeE5ENlN0SlAvZlE2VVNEbTZWdXRFZ0s0clhiQ0luaEVid3RkS3J1RWNqcXJHdjBKM3FBMlhZbmxrVm9CNmY2Zz09";    //ttpodMusic
const QString MUSIC_ALBUM_MULTI_VIP_BD = "VTlieGd5c3g1K1hVSEZwVEFWT2VYcTNMOERJekUrY0pYdWdOVlFkdER2R3RCWUM4ZnlCM1RRclI4Z0gzWTVVeUZIV0F6OUY2Z3lLZ3lmdXN3OEZWbDU4SU9BbkhBT2JoS2lUN09kczR6aVdZaUZZVXg2LzZ0UT09";    //baiduMusic
const QString MUSIC_ALBUM_MULTI_VIP_KW = "UDJKRVdRZmZwalVENkdCalpscUN0SGFOMnBkbFc0QTluRzlxY3BUZ0NXT3kxTGo4aTdRZVRjMVFuTDZsdllDZlNUTXF1UlAyamw1ZHB0Z29NQzgrNlVXWnVjTS9mRDVheDBybjdGODQxdjBsYjgzRWVrR2hxUT09";    //kuwoMusic
const QString MUSIC_ALBUM_MULTI_VIP_KG = "ZzBpL2ZENmgxUDdYcjY3Y1g2cjlPN08zbU13aUxkUWtXME5PbkpVci9wOTh0eGEydDZpYWpRMU1ZK1l4TEhoRGZ2d1hkSVBKWXBJVnRDMDZUdlhYV1ZJOEZ6L29PQWtWTlFRUW82bDY4cXB1UEVGRGdkV05oZz09";    //kuhouMusic


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
