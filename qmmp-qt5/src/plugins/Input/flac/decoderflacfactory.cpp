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

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/tmap.h>
#include <taglib/tfilestream.h>
#include <taglib/id3v2framefactory.h>
#include "cueparser.h"
#include "decoder_flac.h"
#include "flacmetadatamodel.h"
#include "decoderflacfactory.h"

bool DecoderFLACFactory::canDecode(QIODevice *input) const
{
    char buf[36];
    if(input->peek(buf, 36) != 36)
        return false;
    if(!memcmp(buf, "fLaC", 4)) //native flac
        return true;
    if(!memcmp(buf, "OggS", 4) && !memcmp(buf + 29, "FLAC", 4)) //ogg flac
        return true;
    return false;
}

DecoderProperties DecoderFLACFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("FLAC Plugin");
    properties.filters << "*.flac" << "*.oga";
    properties.description = tr("Free Lossless Audio Codec Files");
    properties.contentTypes << "audio/x-flac" << "audio/flac";
    properties.shortName = "flac";
    properties.protocols << "flac";
    return properties;
}

Decoder *DecoderFLACFactory::create(const QString &path, QIODevice *input)
{
    return new DecoderFLAC(path, input);
}

QList<TrackInfo *> DecoderFLACFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredFiles)
{
    //extract metadata of the one cue track
    if(path.contains("://"))
    {
        QString filePath = path;
        filePath.remove("flac://");
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

    TagLib::Ogg::XiphComment *tag = nullptr;
    TagLib::FLAC::Properties *ap = nullptr;

    TagLib::FLAC::File *flacFile = nullptr;
    TagLib::Ogg::FLAC::File *oggFlacFile = nullptr;

    TagLib::FileStream stream(QStringToFileName(path), true);

    if(path.endsWith(".flac", Qt::CaseInsensitive))
    {
        flacFile = new TagLib::FLAC::File(&stream, TagLib::ID3v2::FrameFactory::instance());
        tag = flacFile->xiphComment();
        ap = flacFile->audioProperties();
    }
    else if(path.endsWith(".oga", Qt::CaseInsensitive))
    {
        oggFlacFile = new TagLib::Ogg::FLAC::File(&stream);
        tag = oggFlacFile->tag();
        ap = oggFlacFile->audioProperties();
    }
    else
    {
        delete info;
        return QList<TrackInfo *>();
    }

    if((parts & TrackInfo::MetaData) && tag && !tag->isEmpty())
    {
        if(tag->fieldListMap().contains("CUESHEET"))
        {
            delete info;

            QByteArray data(tag->fieldListMap()["CUESHEET"].toString().toCString(true));
            QString diskNumber;

            if(tag->contains("DISCNUMBER") && !tag->fieldListMap()["DISCNUMBER"].isEmpty())
            {
                TagLib::StringList fld = tag->fieldListMap()["DISCNUMBER"];
                diskNumber = TStringToQString(fld.toString()).trimmed();
            }

            if(flacFile)
                delete flacFile;
            if(oggFlacFile)
                delete oggFlacFile;

            CUEParser parser(data, path);
            if(!diskNumber.isEmpty())
            {
                for(int i = 1; i <= parser.count(); ++i)
                    parser.info(i)->setValue(Qmmp::DISCNUMBER, diskNumber);
            }

            return parser.createPlayList();
        }

        info->setValue(Qmmp::ALBUM, TStringToQString(tag->album()));
        info->setValue(Qmmp::ARTIST, TStringToQString(tag->artist()));
        info->setValue(Qmmp::COMMENT, TStringToQString(tag->comment()));
        info->setValue(Qmmp::GENRE, TStringToQString(tag->genre()));
        info->setValue(Qmmp::TITLE, TStringToQString(tag->title()));
        info->setValue(Qmmp::YEAR, tag->year());
        info->setValue(Qmmp::TRACK, tag->track());
        //additional metadata
        TagLib::StringList fld;
        if(!(fld = tag->fieldListMap()["ALBUMARTIST"]).isEmpty())
            info->setValue(Qmmp::ALBUMARTIST, TStringToQString(fld.toString()));
        if(!(fld = tag->fieldListMap()["COMPOSER"]).isEmpty())
            info->setValue(Qmmp::COMPOSER, TStringToQString(fld.toString()));
        if(!(fld = tag->fieldListMap()["DISCNUMBER"]).isEmpty())
            info->setValue(Qmmp::DISCNUMBER, TStringToQString(fld.toString()));

    }

    if((parts & TrackInfo::Properties) && ap)
    {
        info->setValue(Qmmp::BITRATE, ap->bitrate());
        info->setValue(Qmmp::SAMPLERATE, ap->sampleRate());
        info->setValue(Qmmp::CHANNELS, ap->channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, ap->bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, flacFile ? "FLAC" : "Ogg FLAC");
        info->setDuration(ap->lengthInMilliseconds());
    }

    if((parts & TrackInfo::ReplayGainInfo) && tag && !tag->isEmpty())
    {
        TagLib::Ogg::FieldListMap items = tag->fieldListMap();
        if(items.contains("REPLAYGAIN_TRACK_GAIN"))
            info->setValue(Qmmp::REPLAYGAIN_TRACK_GAIN,TStringToQString(items["REPLAYGAIN_TRACK_GAIN"].front()));
        if(items.contains("REPLAYGAIN_TRACK_PEAK"))
            info->setValue(Qmmp::REPLAYGAIN_TRACK_PEAK,TStringToQString(items["REPLAYGAIN_TRACK_PEAK"].front()));
        if(items.contains("REPLAYGAIN_ALBUM_GAIN"))
            info->setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN,TStringToQString(items["REPLAYGAIN_ALBUM_GAIN"].front()));
        if(items.contains("REPLAYGAIN_ALBUM_PEAK"))
            info->setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK,TStringToQString(items["REPLAYGAIN_ALBUM_PEAK"].front()));
    }

    if(flacFile)
        delete flacFile;
    if(oggFlacFile)
        delete oggFlacFile;
    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderFLACFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    if(!path.contains("://") || path.startsWith("flac://"))
        return new FLACMetaDataModel(path, readOnly);
    else
        return nullptr;
}
