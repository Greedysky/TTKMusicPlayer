#ifndef MUSICNETWORKABSTRACT_H
#define MUSICNETWORKABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QNetworkReply>
#include <QSslConfiguration>
#include "musicnetworkdefines.h"
#include "musicalgorithmutils.h"

/*! @brief The class of abstract downloading data.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicNetworkAbstract : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicNetworkAbstract)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicNetworkAbstract(QObject *parent = nullptr);

    virtual ~MusicNetworkAbstract();

    /*!
     * Release the network object.
     */
    virtual void deleteAll();

    /*!
     * Set network block state.
     */
    inline void setNetworkAbort(bool a) { m_interrupt = a; }
    /*!
     * Get network block state.
     */
    inline bool networkAbort() { return m_interrupt; }

    /*!
     * Set the current raw data.
     */
    inline void setRawData(const QVariantMap &data) { m_rawData = data; }
    /*!
     * Get the current raw data.
     */
    inline const QVariantMap &getRawData() const { return m_rawData; }

Q_SIGNALS:
    /*!
     * Send raw data changed.
     */
    void rawDataChanged(const QVariantMap &data);
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged(const QString &data);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     * Subclass should implement this function.
     */
    virtual void downLoadFinished() = 0;
    /*!
     * Download reply error.
     */
    virtual void replyError(QNetworkReply::NetworkError error);
#ifndef QT_NO_SSL
    /*!
     * Download ssl reply error.
     */
    virtual void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    /*!
     * Download ssl reply error strings.
     */
    void sslErrorsString(QNetworkReply *reply, const QList<QSslError> &errors);
#endif

protected:
    QVariantMap m_rawData;
    MusicObject::NetworkCode m_stateCode;
    volatile bool m_interrupt;
    QNetworkReply *m_reply;
    QNetworkAccessManager *m_manager;

};

namespace MusicObject {
    /*!
     * Set request ssl configuration.
     */
    MUSIC_NETWORK_EXPORT void setSslConfiguration(QNetworkRequest *request, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);

}

#endif // MUSICNETWORKABSTRACT_H
