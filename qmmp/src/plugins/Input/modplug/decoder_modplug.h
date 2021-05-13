/***************************************************************************
 *   Copyright (C) 2008-2021 by Ilya Kotov                                 *
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

#ifndef DECODER_MODPLUG_H
#define DECODER_MODPLUG_H

#include <qmmp/decoder.h>

class CSoundFile;

class DecoderModPlug : public Decoder
{
public:
    explicit DecoderModPlug(const QString &path);
    virtual ~DecoderModPlug();

    void readSettings();
    static DecoderModPlug* instance();

    // Standard Decoder API
    virtual bool initialize() override;
    virtual qint64 totalTime() const override;
    virtual int bitrate() const override;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override;
    virtual void seek(qint64 time) override;

private:
    //helper function
    void deinit();

    CSoundFile *m_soundFile = nullptr;

    int m_bps; //bits per sample
    quint32 m_freq = 0;
    int m_chan = 0, m_sampleSize = 0, m_bitrate = 0;
    qint64 m_totalTime = 0;
    double m_preampFactor = 0.0f;
    bool m_usePreamp = false;
    QString m_path;
    static DecoderModPlug* m_instance;

};

#endif // DECODER_MODPLUG_H
