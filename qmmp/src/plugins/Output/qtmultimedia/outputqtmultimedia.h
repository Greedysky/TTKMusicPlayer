/***************************************************************************
 *   Copyright (C) 2009-2025 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef OUTPUTQTMULTIMEDIA_H
#define OUTPUTQTMULTIMEDIA_H

#include <QObject>
#include <qmmp/output.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class QIODevice;
class OutputControl;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class QAudioOutput;
using QtAudioOutput = QAudioOutput;
#elif QT_VERSION >= QT_VERSION_CHECK(6,2,0)
class QAudioSink;
using QtAudioOutput = QAudioSink;
#endif

class OutputQtMultimedia : public Output
{
public:
    OutputQtMultimedia();
    virtual ~OutputQtMultimedia();

    virtual bool initialize(quint32, ChannelMap map, Qmmp::AudioFormat format) override final;

    virtual qint64 latency() override final;
    virtual qint64 writeAudio(unsigned char *data, qint64 maxSize) override final;
    virtual void drain() override final;
    virtual void reset() override final;
    virtual void suspend() override final;
    virtual void resume() override final;

private:
    QtAudioOutput *m_output = nullptr;
    OutputControl *m_control = nullptr;
    QIODevice *m_buffer = nullptr;
    qint64 m_bytes_per_second = 0;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class OutputControl : public QObject
{
    Q_OBJECT
public:
    explicit OutputControl(QtAudioOutput *o);

public slots:
    void suspend();
    void resume();
    void stop();

private:
    QtAudioOutput *m_output;

};

#endif
