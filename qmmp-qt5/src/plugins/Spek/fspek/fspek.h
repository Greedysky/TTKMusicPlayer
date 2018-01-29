/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#ifndef FSPEK_H
#define FSPEK_H

#include <QThread>

#include <qmmp/spekfactory.h>
#include <qmmp/inputsource.h>
#include <qmmp/decoderfactory.h>
#include <qmmp/qmmpsettings.h>
#include <qmmp/decoder.h>
#include <qmmp/buffer.h>
#include <qmmp/audioconverter.h>
#include <qmmp/recycler_p.h>

class FSpekThead : public QThread
{
    Q_OBJECT
public:
    explicit FSpekThead(QObject *parent = 0);
    ~FSpekThead();

    bool init(const QString &path);
    void stopAndQuitThread();

signals:
    void bufferChanged(Buffer *buffer, int chans);

public slots:
    void start();
    virtual void run() override;

protected:
    qint64 produceSound(unsigned char *data, qint64 size, quint32 brate);
    void flush(bool final);

    int m_sample_size, m_bitrate;
    bool m_finish, m_run;
    quint64 m_output_at, m_output_size, m_bks;
    unsigned char *m_output_buf;
    unsigned int buffer_count;
    AudioConverter m_converter;
    Recycler m_recycler;
    QMutex m_mutex;
    InputSource *m_source;
    Decoder *m_decoder;

};

class FSpek : public Spek
{
    Q_OBJECT
public:
    explicit FSpek(QWidget *parent = 0);
    ~FSpek();

    virtual void open(const QString &path) override;
    virtual void start() override;
    virtual void stop() override;

public slots:
    void bufferChanged(Buffer *buffer, int chans);

protected:
    void paintEvent(QPaintEvent *e);

    void process(float *buffer);
    void draw(QPainter *p);

    QImage m_backgroundImage;
    FSpekThead *m_fspekThread;
    double m_intern_vis_data, m_analyzer_falloff;
    int *m_x_scale, m_buffer_at, m_cols, m_rows, m_pixPos;
    float *m_buffer;
    QMutex m_mutex;

};

#endif
