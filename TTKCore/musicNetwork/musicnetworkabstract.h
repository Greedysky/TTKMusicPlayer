#ifndef MUSICNETWORKABSTRACT_H
#define MUSICNETWORKABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QNetworkReply>
#include <QSslConfiguration>
#include "musiccryptographichash.h"

/*! @brief The class of abstract downloading data.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicNetworkAbstract : public QObject
{
    Q_OBJECT
public:
    explicit MusicNetworkAbstract(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicNetworkAbstract();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void deleteAll();
    /*!
     * Release the network object.
     */

Q_SIGNALS:
    void downLoadDataChanged(const QString &data);
    /*!
     * Send download data from net.
     */

public Q_SLOTS:
    virtual void downLoadFinished() = 0;
    /*!
     * Download data from net finished.
     * Subclass should implement this function.
     */
    virtual void replyError(QNetworkReply::NetworkError error);
    /*!
     * Download reply error.
     */
#ifndef QT_NO_SSL
    virtual void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    /*!
     * Download ssl reply error.
     */
    void sslErrorsString(QNetworkReply *reply, const QList<QSslError> &errors);
    /*!
     * Download ssl reply error strings.
     */
#endif

protected:
    QNetworkReply *m_reply;
    QNetworkAccessManager *m_manager;

};

#endif // MUSICNETWORKABSTRACT_H
