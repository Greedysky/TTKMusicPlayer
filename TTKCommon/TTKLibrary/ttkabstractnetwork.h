#ifndef TTKABSTRACTNETWORK_H
#define TTKABSTRACTNETWORK_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QNetworkReply>
#include <QSslConfiguration>

#include "ttktime.h"
#include "ttksemaphoreloop.h"

/*! @brief The namespace of the network data.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    enum class NetworkCode
    {
        Query = 0xFF00,    /*!< Network state query*/
        Success = 0,       /*!< Network state success*/
        Error = -1,        /*!< Network state error*/
        UnKnow = 2,        /*!< Network state unknow*/
    };
}

/*! @brief The class of the ttk abstract network.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKAbstractNetwork : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(TTKAbstractNetwork)
public:
    /*!
     * Object constructor.
     */
    explicit TTKAbstractNetwork(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~TTKAbstractNetwork();

    /*!
     * Release the network object.
     */
    virtual void deleteAll();

    /*!
     * Set the current raw data.
     */
    inline void setHeader(const QString &key, const QVariant &value) { m_rawData[key] = value; }
    /*!
     * Get the current raw data.
     */
    inline const QVariant header(const QString &key) const { return m_rawData[key]; }

Q_SIGNALS:
    /*!
     * Send download data changed.
     */
    void downLoadDataChanged(const QString &bytes);
    /*!
     * Send download raw data changed.
     */
    void downLoadRawDataChanged(const QByteArray &bytes);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     * Subclass should implement this function.
     */
    virtual void downLoadFinished();
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
    volatile bool m_interrupt;
    volatile TTK::NetworkCode m_stateCode;
    QNetworkReply *m_reply;
    QNetworkAccessManager m_manager;

};

#define TTK_NETWORK_QUERY_CHECK(VALUE)   if(m_interrupt || m_stateCode != TTK::NetworkCode::Query) return VALUE

/*! @brief The namespace of the network helpers.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Make request Content-Type header.
     */
    TTK_MODULE_EXPORT void makeContentTypeHeader(QNetworkRequest *request, const QByteArray &data = {});
    /*!
     * Make request User-Agent header.
     */
    TTK_MODULE_EXPORT void makeUserAgentHeader(QNetworkRequest *request, const QByteArray &data = {});

    /*!
     * Set request ssl configuration.
     */
    TTK_MODULE_EXPORT void setSslConfiguration(QNetworkRequest *request, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);
    /*!
     * Get download file size by url.
     */
    TTK_MODULE_EXPORT qint64 queryFileSizeByUrl(const QString &url);

    /*!
     * Sync network query for get.
     */
    TTK_MODULE_EXPORT QByteArray syncNetworkQueryForGet(QNetworkRequest *request);
    /*!
     * Sync network query for post.
     */
    TTK_MODULE_EXPORT QByteArray syncNetworkQueryForPost(QNetworkRequest *request, const QByteArray &data);

}

#endif // TTKABSTRACTNETWORK_H
