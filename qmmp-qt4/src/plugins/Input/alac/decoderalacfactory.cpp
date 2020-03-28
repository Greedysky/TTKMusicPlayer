/*
 * ALAC (Apple Lossless Audio Codec) decoder
 * Copyright (c) 2005 David Hammerton
 * All rights reserved.
 *
 * This is the actual decoder.
 *
 * http://crazney.net/programs/itunes/alac.html
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <QtPlugin>
#include "decoderalacfactory.h"
#include "decoder_alac.h"
#include "alachelper.h"

bool DecoderALACFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderALACFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("ALAC Plugin");
    properties.filters << "*.alac";
    properties.description = tr("ALAC Files");
    properties.shortName = "alac";
    properties.hasSettings = false;
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderALACFactory::create(const QString &path, QIODevice *)
{
    return new DecoderALAC(path);
}

QList<TrackInfo *> DecoderALACFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    ALACHelper alac(path.toUtf8().constData());
    if(!alac.initialize())
    {
        return QList <TrackInfo *>();
    }

    TrackInfo *info = new TrackInfo(path);
    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, alac.bitrate());
        info->setValue(Qmmp::SAMPLERATE, alac.samplerate());
        info->setValue(Qmmp::CHANNELS, alac.channels());
        info->setDuration(alac.totalTime());
    }

    return QList <TrackInfo *>() << info;
}

MetaDataModel* DecoderALACFactory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}

Q_EXPORT_PLUGIN2(alac,DecoderALACFactory)
