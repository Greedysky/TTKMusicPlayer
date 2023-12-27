/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
 ***************************************************************************/

#ifndef HTTPSTREAMREADER_H
#define HTTPSTREAMREADER_H

#include <QMutex>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

/*!
 * @author Greedysky <greedysky@163.com>
 */
struct StreamData
{
    QByteArray buffer;
    size_t buffer_fill = 0;
    size_t buffer_size = 0;
    QString content_type;
    bool aborted = false;
};

/*!
 * @author Greedysky <greedysky@163.com>
 */
class HttpStreamReader : public QIODevice
{
    Q_OBJECT
public:
    explicit HttpStreamReader(const QString &url, QObject *parent);
    virtual ~HttpStreamReader();

    /**
     *  QIODevice API
     */
    virtual bool atEnd() const override final;
    virtual qint64 bytesAvailable() const override final;
    virtual qint64 bytesToWrite() const override final;
    virtual void close() override final;
    virtual bool isSequential() const override final;
    virtual bool open(OpenMode mode) override final;
    virtual bool seek(qint64 time) override final;

    /**
     *  helper functions
     */
    void run();
    QString contentType() const;

signals:
    void ready();
    void error();

private slots:
    void replyError(QNetworkReply::NetworkError status);
    void handleReadyRead();
    void handleFinished();

protected:
    virtual qint64 readData(char *, qint64) override final;
    virtual qint64 writeData(const char *, qint64) override final;

private:
    void abort();
    qint64 readBuffer(char* data, qint64 maxlen);

    QString m_url;
    QString m_path;
    QMutex m_mutex;
    StreamData m_stream;
    bool m_ready = false;
    size_t m_bufferSize = 0;

    QNetworkReply *m_reply = nullptr;
    QNetworkAccessManager m_manager;

};

#endif
