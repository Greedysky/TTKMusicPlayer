#ifndef MUSICDOWNLOADTHREADABSTRACT_H
#define MUSICDOWNLOADTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QFile>
#include <QTimer>
#include "musicobject.h"
#include "musicnetworkabstract.h"
#include "musicnumberdefine.h"

/*! @brief The class of abstract downloading data.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadThreadAbstract : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    enum Download_Type
    {
        Download_Music, ///*type of dwonlaod music*/
        Download_Lrc,   ///*type of dwonlaod lrc*/
        Download_SmlBG, ///*type of dwonlaod small background*/
        Download_BigBG, ///*type of dwonlaod big background*/
        Download_Video, ///*type of dwonlaod video*/
        Download_Other  ///*type of dwonlaod other user mod*/
    };

    MusicDownLoadThreadAbstract(const QString &url, const QString &save,
                                Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */
    virtual ~MusicDownLoadThreadAbstract();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void deleteAll() override;
    /*!
     * Release the network object.
     */
    virtual void startToDownload() = 0;
    /*!
     * Start to download data.
     * Subclass should implement this function.
     */

public Q_SLOTS:
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    /*!
     * Get download received and total data.
     */
    virtual void replyError(QNetworkReply::NetworkError error) override;
    /*!
     * Download reply error.
     */
#ifndef QT_NO_SSL
    virtual void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) override;
    /*!
     * Download ssl reply error.
     */
#endif
    virtual void updateDownloadSpeed();
    /*!
     * Updata download speed due the user mod the net speed limited.
     */

protected:
    QString transferData() const;
    /*!
     * Transfer the enum type to string.
     */

    QFile *m_file;
    QString m_url, m_savePathName;
    Download_Type m_downloadType;
    qint64 m_hasReceived, m_currentReceived, m_totalSize;
    QTimer m_timer;

};

#endif // MUSICDOWNLOADTHREADABSTRACT_H
