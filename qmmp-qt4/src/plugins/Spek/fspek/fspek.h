/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
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

    int m_pixPos;
    QImage m_backgroundImage;
    FSpekThead *m_fspekThread;
    double m_intern_vis_data;
    int *m_x_scale, m_buffer_at, m_cols, m_rows;
    double m_analyzer_falloff;
    float *m_buffer;
    QMutex m_mutex;

};

#endif
