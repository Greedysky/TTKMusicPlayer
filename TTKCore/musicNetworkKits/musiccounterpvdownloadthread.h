#ifndef MUSICCOUNTERPVDOWNLOADTHREAD_H
#define MUSICCOUNTERPVDOWNLOADTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicnetworkabstract.h"

const QString QURTY_URL  = "S1AwTklDeUNQdG95dkIrNTdzZ1lBR3hMSFFqeTlGTjJjTlo4M3hYaHAyMHV3dTBFZ0x2R1dJVXJvOFN4REphRTFBS3BFeThHUzU3QVRjMHErTEFyZ2Y3N1NPRW0zYXh6anV2VGtUYzNUb3p6NHY0eQ==";
const QString HOST_URL   = "WUwzSXNrdXhZS2owUEpzWTN3V3NOVUNPTkZNdHBMWDY=";
const QString REFER_URL  = "a0RUSDNPMnFabWQwOVhVa2JFSkVtM0JCazVKRTIrbVFJWEpKd2g0Y1NLbz0=";
const QString COOKIE_URL = "cWpxdUI0YmIyNDdOcWp6TWZvMy9hYkdNN25UYVdpdStSUEV6Q3ZpMzNkRjd4WjV0ZmxzYnYxejRiTVJlUG5Wdi9UazlweFJPZGJlbVVpdGltTndId3NxNFUrSHNUcUQ5OENMdENnaUFOcElvTkpVcFVsRTQvS2Z3MndLNTNBb3Q=";

/*! @brief The class of get counter pv of player.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicCounterPVDownloadThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    explicit MusicCounterPVDownloadThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicCounterPVDownloadThread();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void startToDownload();
    /*!
     * Start to download counter pv from net.
     */

public Q_SLOTS:
    virtual void downLoadFinished();
    /*!
     * Download data from net finished.
     */
};

#endif // MUSICCOUNTERPVDOWNLOADTHREAD_H
