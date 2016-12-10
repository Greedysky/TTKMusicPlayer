#ifndef GETBAIDUCHANNEL_H
#define GETBAIDUCHANNEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicradiothreadabstract.h"

typedef struct MUSIC_NETWORK_EXPORT ChannelInfo
{
    QString m_id;
    QString m_name;
}ChannelInfo;
TTK_DECLARE_LISTS(ChannelInfo)

/*! @brief The class of music radio thread of song channel.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioChannelThread : public MusicRadioThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicRadioChannelThread(QObject *parent = 0, QNetworkCookieJar *cookie = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicRadioChannelThread();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startToDownload(const QString &id) override;
    /*!
     * Start to download data.
     */
    ChannelInfos getMusicChannel();
    /*!
     * Get music channel.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    ChannelInfos m_channels;

};

#endif // GETBAIDUCHANNEL_H
