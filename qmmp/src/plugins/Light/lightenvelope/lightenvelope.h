/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#ifndef LIGHTENVELOPE_H
#define LIGHTENVELOPE_H

#include <QThread>
#include <QMutex>
#include <QPixmap>
#include <qmmp/lightfactory.h>
#include <qmmp/audioparameters.h>

class Decoder;
class InputSource;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class LightEnvelopeScanner : public QThread
{
    Q_OBJECT
public:
    explicit LightEnvelopeScanner(QObject *parent);
    ~LightEnvelopeScanner();

    bool scan(const QString &path);
    void stop();

    const QList<int> &data() const;
    const AudioParameters &audioParameters() const;

signals:
    void dataChanged();

private:
    virtual void run() override;

    bool m_user_stop = false;
    Decoder *m_decoder = nullptr;
    InputSource *m_input = nullptr;
    mutable QMutex m_mutex;
    QList<int> m_data;
    AudioParameters m_ap;

};

/*!
 * @author Greedysky <greedysky@163.com>
 */
class LightEnvelope : public Light
{
    Q_OBJECT
public:
    explicit LightEnvelope(QWidget *parent = nullptr);
    ~LightEnvelope();

    virtual void open(const QString &path) override;
    virtual void start() override;
    virtual void stop() override;

private slots:
    void readSettings();
    void writeSettings();
    void scanFinished();
    void dataChanged();
    void mediaUrlChanged();
    void positionChanged(qint64 elapsed);

private:
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void contextMenuEvent(QContextMenuEvent *e) override;

    void drawWaveform();

    LightEnvelopeScanner *m_scanner = nullptr;
    QAction *m_showTwoChannelsAction, *m_showRmsAction;
    QList<int> m_data;
    int m_channels = 0;
    qint64 m_elapsed = 0;
    qint64 m_duration = 0;
    QPixmap m_pixmap;

};

#endif
