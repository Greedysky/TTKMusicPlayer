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

/*! @brief The class of the radio channel info item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_NETWORK_EXPORT MusicRadioChannelInfo
{
    QString m_id;
    QString m_name;
}MusicRadioChannelInfo;
TTK_DECLARE_LISTS(MusicRadioChannelInfo)

/*! @brief The class of music radio thread of song channel.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioChannelThread : public MusicRadioThreadAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRadioChannelThread(QObject *parent = 0, QNetworkCookieJar *cookie = 0);

    virtual ~MusicRadioChannelThread();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download data.
     */
    virtual void startToDownload(const QString &id) override;
    /*!
     * Get music channel.
     */
    MusicRadioChannelInfos getMusicChannel();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    MusicRadioChannelInfos m_channels;

};

#endif // GETBAIDUCHANNEL_H
