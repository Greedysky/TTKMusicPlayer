#ifndef MUSICSOURCEUPDATETHREAD_H
#define MUSICSOURCEUPDATETHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QVariantMap>
#include "musicversion.h"
#include "musicglobaldefine.h"

#define VERSION_URL     "TzFTSjRiNlN6TXB3WGZtdENONGdaQzhxdkpNc0ZXZk9VSUtXdlhQQ252bWJLZ0pidE5LcW80WUlHQmQzNWp0OGxXSjFkQT09"
#define DOWNLOAD_URL    "YXhxRk5PeWpscVNYbEZKMmEwbUExdkMxcm9QN1ZybTlZYTcwVmN1aUdTNEJoMFRiM3V5cnE2S3VDbG89"

/*! @brief The class of source data query pdate thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicSourceUpdateThread : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSourceUpdateThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download data.
     */
    void startToDownload();

    /*!
     * Get lasted version.
     */
    QString getLastedVersion() const;
    /*!
     * Is lasted version.
     */
    bool isLastedVersion() const;
    /*!
     * Get lasted version description.
     */
    QString getLastedVersionDes() const;

Q_SIGNALS:
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged(const QVariant &data);

public Q_SLOTS:
    /*!
     * Download data from kuwo net finished.
     */
    void downLoadFinished(const QByteArray &data);

protected:
    QVariantMap m_rawData;

};

#endif // MUSICSOURCEUPDATETHREAD_H
