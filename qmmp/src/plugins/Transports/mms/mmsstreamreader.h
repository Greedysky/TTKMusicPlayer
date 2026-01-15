/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#ifndef MMSSTREAMREADER_H
#define MMSSTREAMREADER_H

#include <QMutex>
#include <QProcess>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class MMSStreamReader : public QIODevice
{
    Q_OBJECT
public:
    explicit MMSStreamReader(const QString &url, QObject *parent);
    virtual ~MMSStreamReader();

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

signals:
    void ready();
    void error();

private slots:
    void handleError();
    void handleReadyRead();

protected:
    virtual qint64 readData(char *, qint64) override final;
    virtual qint64 writeData(const char *, qint64) override final;

private:
    void abort();
    qint64 readBuffer(char* data, qint64 maxlen);

private:
    struct Data
    {
        QByteArray buffer;
        size_t fill = 0;
        size_t size = 0;
        bool aborted = false;
    } m_stream;

    QString m_url;
    QMutex m_mutex;
    bool m_ready = false;
    size_t m_bufferSize = 0;
    QProcess *m_process = nullptr;

};

#endif
