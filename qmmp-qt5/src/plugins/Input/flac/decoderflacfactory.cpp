/***************************************************************************
 *   Copyright (C) 2008-2016 by Ilya Kotov                                 *
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

// DecoderFLACFactory

bool DecoderFLACFactory::supports(const QString &source) const
{
    return source.endsWith(".flac", Qt::CaseInsensitive) || source.endsWith(".oga", Qt::CaseInsensitive);
}

bool DecoderFLACFactory::canDecode(QIODevice *input) const
{
    char buf[36];
    if (input->peek(buf, 36) != 36)
        return false;
    if(!memcmp(buf, "fLaC", 4)) //native flac
        return true;
    if(!memcmp(buf, "OggS", 4) && !memcmp(buf + 29, "FLAC", 4)) //ogg flac
        return true;
    return false;
}

const DecoderProperties DecoderFLACFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("FLAC Plugin");
    properties.filters << "*.flac" << "*.oga";
    properties.description = tr("FLAC Files");
    properties.contentTypes << "audio/x-flac" << "audio/flac";
    properties.shortName = "flac";
    properties.protocols << "flac";
    properties.hasAbout = true;
    properties.hasSettings = false;
    return properties;
}

Decoder *DecoderFLACFactory::create(const QString &path, QIODevice *i)
{
    return new DecoderFLAC(path, i);
}

QList<FileInfo *> DecoderFLACFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *ignoredFiles)
{
    QList <FileInfo*> list;
    TagLib::Ogg::XiphComment *tag = 0;
    TagLib::FLAC::Properties *ap = 0;

    TagLib::FLAC::File *flacFile = 0;
    TagLib::Ogg::FLAC::File *oggFlacFile = 0;

    //extract metadata of the one cue track
    if(fileName.contains("://"))
    {
        QString path = fileName;
        path.remove("flac://");
        path.remove(QRegExp("#\\d+$"));
        int track = fileName.section("#", -1).toInt();
        list = createPlayList(path, true, ignoredFiles);
        if (list.isEmpty() || track <= 0 || track > list.count())
        {
            qDeleteAll(list);
            list.clear();
            return list;
        }
        FileInfo *info = list.takeAt(track - 1);
        qDeleteAll(list);
        return QList<FileInfo *>() << info;
    }

    TagLib::FileStream stream(QStringToFileName(fileName), true);

    if(fileName.endsWith(".flac", Qt::CaseInsensitive))
    {
        flacFile = new TagLib::FLAC::File(&stream, TagLib::ID3v2::FrameFactory::instance());
        tag = useMetaData ? flacFile->xiphComment() : 0;
        ap = flacFile->audioProperties();
    }
    else if(fileName.endsWith(".oga", Qt::CaseInsensitive))
    {
        oggFlacFile = new TagLib::Ogg::FLAC::File(&stream);
        tag = useMetaData ? oggFlacFile->tag() : 0;
        ap = oggFlacFile->audioProperties();
    }
    else
        return list;

    FileInfo *info = new FileInfo(fileName);
    if (tag && !tag->isEmpty())
    {
        info->setMetaData(Qmmp::ALBUM,
                          QString::fromUtf8(tag->album().toCString(true)).trimmed());
        info->setMetaData(Qmmp::ARTIST,
                          QString::fromUtf8(tag->artist().toCString(true)).trimmed());
        info->setMetaData(Qmmp::COMMENT,
                          QString::fromUtf8(tag->comment().toCString(true)).trimmed());
        info->setMetaData(Qmmp::GENRE,
                          QString::fromUtf8(tag->genre().toCString(true)).trimmed());
        info->setMetaData(Qmmp::TITLE,
                          QString::fromUtf8(tag->title().toCString(true)).trimmed());
        info->setMetaData(Qmmp::YEAR, tag->year());
        info->setMetaData(Qmmp::TRACK, tag->track());

        if (tag->fieldListMap().contains("CUESHEET"))
        {
            CUEParser parser(tag->fieldListMap()["CUESHEET"].toString().toCString(true), fileName);
            if(tag->contains("DISCNUMBER") && !tag->fieldListMap()["DISCNUMBER"].isEmpty())
            {
                TagLib::StringList fld = tag->fieldListMap()["DISCNUMBER"];
                for(int i = 1; i <= parser.count(); i++)
                {
                    parser.info(i)->setMetaData(Qmmp::DISCNUMBER,
                              QString::fromUtf8(fld.toString().toCString(true)).trimmed());
                }
            }
            list = parser.createPlayList();
            delete info;
            if(flacFile)
                delete flacFile;
            if(oggFlacFile)
                delete oggFlacFile;
            return list;
        }

        //additional metadata
        TagLib::StringList fld;
        if(!(fld = tag->fieldListMap()["ALBUMARTIST"]).isEmpty())
            info->setMetaData(Qmmp::ALBUMARTIST,
                              QString::fromUtf8(fld.front().toCString(true)).trimmed());
        if(!(fld = tag->fieldListMap()["COMPOSER"]).isEmpty())
            info->setMetaData(Qmmp::COMPOSER,
                              QString::fromUtf8(fld.front().toCString(true)).trimmed());
        if(!(fld = tag->fieldListMap()["DISCNUMBER"]).isEmpty())
            info->setMetaData(Qmmp::DISCNUMBER,
                              QString::fromUtf8(fld.front().toCString(true)).trimmed());
    }
    if(ap)
        info->setLength(ap->length());
    list << info;
    if(flacFile)
        delete flacFile;
    if(oggFlacFile)
        delete oggFlacFile;
    return list;
}

MetaDataModel*DecoderFLACFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    if (!path.contains("://") || path.startsWith("flac://"))
        return new FLACMetaDataModel(path, parent);
    else
        return 0;
}
