/***************************************************************************
 *   Copyright (C) 2008-2019 by Ilya Kotov                                 *
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

#include <QtPlugin>
#include "wavpackmetadatamodel.h"
#include "decoder_wavpack.h"
#include "decoderwavpackfactory.h"
#include "cueparser.h"

bool DecoderWavPackFactory::canDecode(QIODevice *input) const
{
    char buf[4];
    return (input->peek(buf, 4) == 4 && !memcmp(buf, "wvpk", 4));
}

DecoderProperties DecoderWavPackFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("WavPack Plugin");
    properties.filters << "*.wv";
    properties.description = tr("WavPack Files");
    properties.contentTypes << "audio/x-wavpack";
    properties.shortName = "wavpack";
    properties.noInput = true;
    properties.protocols << "file" << "wvpack";
    return properties;
}

Decoder *DecoderWavPackFactory::create(const QString &path, QIODevice *)
{
    return new DecoderWavPack(path);
}

QList<TrackInfo *> DecoderWavPackFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredFiles)
{
    //extract metadata of one cue track
    if(path.contains("://"))
    {
        QString filePath = path;
        filePath.remove("wvpack://");
        filePath.remove(QRegExp("#\\d+$"));
        int track = path.section("#", -1).toInt();
        QList<TrackInfo *> list = createPlayList(filePath, TrackInfo::AllParts, ignoredFiles);
        if(list.isEmpty() || track <= 0 || track > list.count())
        {
            qDeleteAll(list);
            list.clear();
            return list;
        }
        TrackInfo *info = list.takeAt(track - 1);
        qDeleteAll(list);
        return QList<TrackInfo *>() << info;
    }

    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
        return QList<TrackInfo *>() << info;

    char err[80] = { 0 };

#if defined(Q_OS_WIN) && defined(OPEN_FILE_UTF8)
    WavpackContext *ctx = WavpackOpenFileInput (path.toUtf8().constData(),
                                                err, OPEN_WVC | OPEN_TAGS | OPEN_FILE_UTF8, 0);
#else
    WavpackContext *ctx = WavpackOpenFileInput (path.toLocal8Bit().constData(),
                                                err, OPEN_WVC | OPEN_TAGS, 0);
#endif
    if(!ctx)
    {
        qWarning("DecoderWavPackFactory: error: %s", err);
        delete info;
        return QList<TrackInfo *>();
    }

    if(parts & TrackInfo::MetaData)
    {
        int cue_len = WavpackGetTagItem (ctx, "cuesheet", nullptr, 0);
        if(cue_len > 0)
        {
            delete info;
            char value[cue_len + 1];
            memset(value, 0, cue_len + 1);
            WavpackGetTagItem(ctx, "cuesheet", value, cue_len + 1);
            WavpackCloseFile(ctx);
            CUEParser parser(value, path);
            return parser.createPlayList();
        }
        else
        {
            char value[200] = { 0 };
            WavpackGetTagItem (ctx, "Album", value, sizeof(value));
            info->setValue(Qmmp::ALBUM, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Artist", value, sizeof(value));
            info->setValue(Qmmp::ARTIST, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Album Artist", value, sizeof(value));
            info->setValue(Qmmp::ALBUMARTIST, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Comment", value, sizeof(value));
            info->setValue(Qmmp::COMMENT, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Genre", value, sizeof(value));
            info->setValue(Qmmp::GENRE, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Title", value, sizeof(value));
            info->setValue(Qmmp::TITLE, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Composer", value, sizeof(value));
            info->setValue(Qmmp::COMPOSER, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Year", value, sizeof(value));
            info->setValue(Qmmp::YEAR, QString::fromUtf8(value).toInt());
            WavpackGetTagItem (ctx, "Track", value, sizeof(value));
            info->setValue(Qmmp::TRACK, QString::fromUtf8(value).toInt());
            WavpackGetTagItem (ctx, "Disc", value, sizeof(value));
            info->setValue(Qmmp::DISCNUMBER, QString::fromUtf8(value).toInt());
        }
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, WavpackGetAverageBitrate(ctx, 1));
        info->setValue(Qmmp::SAMPLERATE, WavpackGetSampleRate(ctx));
        info->setValue(Qmmp::CHANNELS, WavpackGetNumChannels(ctx));
        info->setValue(Qmmp::BITS_PER_SAMPLE, WavpackGetBitsPerSample(ctx));
        info->setValue(Qmmp::FORMAT_NAME, "WavPack");
        info->setDuration((qint64)WavpackGetNumSamples(ctx) * 1000 / WavpackGetSampleRate(ctx));
    }

    if(parts & TrackInfo::ReplayGainInfo)
    {
        char value[200] = { 0 };
        WavpackGetTagItem(ctx, "REPLAYGAIN_TRACK_GAIN", value, sizeof(value));
        info->setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, value);
        WavpackGetTagItem(ctx, "REPLAYGAIN_TRACK_PEAK", value, sizeof(value));
        info->setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, value);
        WavpackGetTagItem(ctx, "REPLAYGAIN_ALBUM_GAIN", value, sizeof(value));
        info->setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN, value);
        WavpackGetTagItem(ctx, "REPLAYGAIN_ALBUM_PEAK", value, sizeof(value));
        info->setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK, value);
    }

    WavpackCloseFile (ctx);
    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderWavPackFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    if(!path.contains("://") || path.startsWith("wvpack://"))
        return new WavPackMetaDataModel(path, readOnly);
    else
        return nullptr;
}

Q_EXPORT_PLUGIN2(wavpack,DecoderWavPackFactory)
