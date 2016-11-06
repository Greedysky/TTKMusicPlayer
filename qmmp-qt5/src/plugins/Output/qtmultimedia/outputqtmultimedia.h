/***************************************************************************
 *   Copyright (C) 2015 by Ivan Ponomarev                                  *
 *   ivantrue@gmail.com                                                    *
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

#include <qmmp/output.h>
#include <QScopedPointer>
#include <QObject>

/**
    @author Ivan Ponomarev ivantrue@gmail.com
*/
class QAudioOutput;
class QIODevice;
class OutputControl;

class OutputQtMultimedia : public Output
{
public:
    OutputQtMultimedia();
    ~OutputQtMultimedia();

    virtual bool initialize(quint32, ChannelMap map, Qmmp::AudioFormat format);
    virtual qint64 latency();
    virtual qint64 writeAudio(unsigned char *data, qint64 maxSize);
    virtual void drain();
    virtual void reset();
    virtual void suspend();
    virtual void resume();

private:
    QScopedPointer<QAudioOutput> m_output;
    QScopedPointer<OutputControl> m_control;
    QIODevice *m_buffer;
    qint64 m_bytes_per_second;
};

class OutputControl : public QObject
{
    Q_OBJECT

public:
    OutputControl(QAudioOutput *o);

public slots:
    void suspend();
    void resume();

private:
    QAudioOutput *m_output;

};


#endif // OUTPUTQTMULTIMEDIA_H
