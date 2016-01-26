/***************************************************************************
 *   Copyright (C) 2008-2009 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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

#ifndef DECODER_MODPLUG_H
#define DECODER_MODPLUG_H

#include <qmmp/decoder.h>

class CSoundFile;

class DecoderModPlug : public Decoder
{
public:
    DecoderModPlug(const QString &path);
    virtual ~DecoderModPlug();

    void readSettings();
    static DecoderModPlug* instance();
    // Standard Decoder API
    bool initialize();
    qint64 totalTime();
    int bitrate();
    qint64 read(char *audio, qint64 maxSize);
    void seek(qint64 time);

private:

    //helper function
    void deinit();

    CSoundFile *m_soundFile;

    int m_bps; //bits per sample
    QByteArray m_input_buf; //input buffer
    quint32 m_freq;
    int m_chan, m_sampleSize, m_bitrate;
    qint64 m_totalTime;
    double m_preampFactor;
    bool m_usePreamp;
    QString m_path;
    static DecoderModPlug* m_instance;
};

#endif // DECODER_MODPLUG_H
