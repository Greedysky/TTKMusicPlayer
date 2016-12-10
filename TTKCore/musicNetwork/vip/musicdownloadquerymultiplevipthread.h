#ifndef MUSICDOWNLOADQUERYMULTIPLEVIPTHREAD_H
#define MUSICDOWNLOADQUERYMULTIPLEVIPTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

//mulity query
const QString MUSIC_REQUERY_MULTI_VIP_WY = "VC9pYkJZYzlRcTBiTWxiRmNoN3djUVNJL2NsMzJBcjM0MENOeXlXUTlyaUxzUHBvQzg3T0VSQWpnYlpPRmFtUXptbW8wL0MyNktyUk81VXQ=";    //wangyiMusic
const QString MUSIC_REQUERY_MULTI_VIP_QQ = "eHU3RXpsZmM0ZVdLNzBVV0VxeU4zeC9raEdGMmdvSHBIQm5QNjVMRjIrRnZjNTBZdHk0OVpQOEVVaWh3RHpHM2tTVlZRYjZXNmYzZ3JMSXg=";    //qqMusic
const QString MUSIC_REQUERY_MULTI_VIP_XM = "d2QvMm84bWp2bGpzYnRyZHNPTWxRMHVuN2QxejBhczVDK1hyOStqbjdCZ1Q0eFdjUktEZ3R4bkFyZDZ2eHdmOURnOFBXK2NLQkRpYWNnaGQ=";    //xiamiMusic
const QString MUSIC_REQUERY_MULTI_VIP_TT = "ZldncGs5S0hzcmJUL3Z6MmNId0RVaVR0eTZ5SVlzODQyNUFpODJTRzVPNDRRQWd6VXA1NUM1bWRheHFnVVFRMnphTFZ5ckIyMk9Ha2MvWmo=";    //ttpodMusic
const QString MUSIC_REQUERY_MULTI_VIP_BD = "d3R0emhOYXlHbXhsejRSQW8vQStoT3FoRGJ0UUhpRkN0QUp5OGMrcHRSNW1LMzJPRG1XTWlURHdQMWFTWjVSa3NEUE84ejlWcGpjRFkya1Y=";    //baiduMusic
const QString MUSIC_REQUERY_MULTI_VIP_KW = "UzFjc1UxQ0UzcGhsTGplRHhCanZwTUpkdGtVYmY2dXBLZmgvYWpyNHlwSHNLaDZrYWpCN1BYc3FnMmFDYlhiLzAxQnk0djdhRkIwc2pWYXA=";    //kuwoMusic
const QString MUSIC_REQUERY_MULTI_VIP_KG = "Z2txTnVleWVzSnVZOG9aVDF2YUpmQ01YL2UxdjZJTDNabXNRdDd5YkZIQ1FIQkszMnV3NnVIcFJzRmplWjdHdnhCQzV3SS9oVFBsRGozbEo=";    //kuhouMusic


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
