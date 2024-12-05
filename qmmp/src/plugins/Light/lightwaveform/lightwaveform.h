/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#ifndef LIGHTWAVEFORM_H
#define LIGHTWAVEFORM_H

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
class LightWaveFormScanner : public QThread
{
    Q_OBJECT
public:
    explicit LightWaveFormScanner(QObject *parent);
    ~LightWaveFormScanner();

    bool scan(const QString &path);
    void stop();

    const QList<int> &data() const;
    const AudioParameters &audioParameters() const;

signals:
    void dataChanged();

private:
    virtual void run() override final;

    bool m_stop = false;
    Decoder *m_decoder = nullptr;
    InputSource *m_input = nullptr;
    mutable QMutex m_mutex;
    QList<int> m_data;
    AudioParameters m_ap;

};

/*!
 * @author Greedysky <greedysky@163.com>
 */
class LightWaveForm : public Light
{
    Q_OBJECT
public:
    explicit LightWaveForm(QWidget *parent = nullptr);
    ~LightWaveForm();

    virtual void open(const QString &path) override final;
    virtual void start() override final;
    virtual void stop() override final;

private slots:
    void readSettings();
    void writeSettings();
    void scanFinished();
    void dataChanged();
    void mediaUrlChanged();
    void positionChanged(qint64 elapsed);
    void typeChanged(QAction *action);

private:
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void resizeEvent(QResizeEvent *e) override final;
    virtual void mousePressEvent(QMouseEvent *e) override final;
    virtual void mouseMoveEvent(QMouseEvent *e) override final;
    virtual void mouseReleaseEvent(QMouseEvent *e) override final;
    virtual void contextMenuEvent(QContextMenuEvent *e) override final;

    void drawWaveform();

    LightWaveFormScanner *m_scanner = nullptr;
    QList<QColor> m_colors;
    QAction *m_channelsAction, *m_rmsAction;
    QAction *m_rulerAction, *m_logScaleAction;
    QAction *m_shadeAction, *m_cloudAction;
    QAction *m_pointsAction, *m_fatAction;
    QList<int> m_data;
    int m_channels = 0;
    qint64 m_elapsed = 0;
    qint64 m_duration = 0;
    QPixmap m_pixmap;
    int m_seekPos = -1;

};

#endif
