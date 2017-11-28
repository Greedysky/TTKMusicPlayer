/***************************************************************************
 *   Copyright (C) 2008-2017 by Ilya Kotov                                 *
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

#include <QFile>
#include <QTextCodec>
#include <mad.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/apetag.h>
#include <taglib/tfile.h>
#include <taglib/mpegfile.h>
#include <taglib/tfilestream.h>
#include "mpegmetadatamodel.h"
#include "replaygainreader.h"
#include "decoder_mad.h"
#include "decodermadfactory.h"

// DecoderMADFactory

DecoderMADFactory::DecoderMADFactory()
{
    //detecting rusxmms patch
    m_using_rusxmms = false;
    char str[] = { char(0xF2), char(0xE5), char(0xF1), char(0xF2), '\0'};
    QTextCodec *codec = QTextCodec::codecForName ("windows-1251");
    TagLib::String tstr(str);
    if(codec->toUnicode(str) == QString::fromUtf8(tstr.toCString(true)))
    {
        qDebug("DecoderMADFactory: found taglib with rusxmms patch");
        m_using_rusxmms = true;
    }
}

bool DecoderMADFactory::canDecode(QIODevice *input) const
{
    char buf[8192];
    qint64 buf_at = sizeof(buf);

    if(input->peek(buf, sizeof(buf)) != sizeof(buf))
        return false;

    if (!memcmp(buf, "FLV", 3)) //skip Macromedia Flash Video
        return false;

    if (!memcmp(buf + 8, "WAVE", 4))
            return !memcmp(buf + 20, "U" ,1);

    if(!memcmp(buf, "ID3", 3))
    {
        TagLib::ByteVector byteVector(buf, sizeof(buf));
        TagLib::ID3v2::Header header(byteVector);

        //skip id3v2tag if possible
        if(input->isSequential())
        {
            if(header.tagSize() >= sizeof(buf))
                return false;

            buf_at = sizeof(buf) - header.tagSize();
            memmove(buf, buf + header.tagSize() + header.tagSize(), sizeof(buf) - header.tagSize());
        }
        else
        {
            input->seek(header.tagSize());
            buf_at = input->read(buf, sizeof(buf));
            input->seek(0); //restore inital position
        }
    }

    if(buf_at > 0)
    {
        struct mad_stream stream;
        struct mad_header header;
        int dec_res;

        mad_stream_init (&stream);
        mad_header_init (&header);
        mad_stream_buffer (&stream, (unsigned char *) buf, buf_at);
        stream.error = MAD_ERROR_NONE;

        while ((dec_res = mad_header_decode(&header, &stream)) == -1
               && MAD_RECOVERABLE(stream.error))
            ;
        return dec_res != -1 ? true: false;
    }
    return false;
}

const DecoderProperties DecoderMADFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("MPEG Plugin");
    properties.shortName = "mad";
    properties.filters << "*.mp1" << "*.mp2" << "*.mp3" << "*.wav";
    properties.description = tr("MPEG Files");
    properties.contentTypes << "audio/mp3" << "audio/mpeg";
    properties.hasAbout = true;
    properties.hasSettings = true;
    return properties;
}

Decoder *DecoderMADFactory::create(const QString &url, QIODevice *input)
{
    Decoder *d = new DecoderMAD(input);
    if(!url.contains("://")) //local file
    {
        ReplayGainReader rg(url);
        d->setReplayGainInfo(rg.replayGainInfo());
    }
    return d;
}

QList<FileInfo *> DecoderMADFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *)
{
    FileInfo *info = new FileInfo(fileName);
    TagLib::Tag *tag = 0;

    TagLib::FileStream stream(QStringToFileName(fileName), true);
    TagLib::MPEG::File fileRef(&stream, TagLib::ID3v2::FrameFactory::instance());

    if (useMetaData)
    {
        QTextCodec *codec = 0;

        if(m_using_rusxmms)
            codec = QTextCodec::codecForName ("UTF-8");

        if (!codec)
            codec = QTextCodec::codecForName ("UTF-8");

        if (tag && codec)
        {
            bool utf = codec->name ().contains("UTF");
            TagLib::String album = tag->album();
            TagLib::String artist = tag->artist();
            TagLib::String comment = tag->comment();
            TagLib::String genre = tag->genre();
            TagLib::String title = tag->title();

            info->setMetaData(Qmmp::ALBUM,
                              codec->toUnicode(album.toCString(utf)).trimmed());
            info->setMetaData(Qmmp::ARTIST,
                              codec->toUnicode(artist.toCString(utf)).trimmed());
            info->setMetaData(Qmmp::COMMENT,
                              codec->toUnicode(comment.toCString(utf)).trimmed());
            info->setMetaData(Qmmp::GENRE,
                              codec->toUnicode(genre.toCString(utf)).trimmed());
            info->setMetaData(Qmmp::TITLE,
                              codec->toUnicode(title.toCString(utf)).trimmed());
            info->setMetaData(Qmmp::YEAR,
                              tag->year());
            info->setMetaData(Qmmp::TRACK,
                              tag->track());

            if(tag == fileRef.ID3v2Tag())
            {
                if(!fileRef.ID3v2Tag()->frameListMap()["TPE2"].isEmpty())
                {
                    TagLib::String albumArtist;
                    albumArtist = fileRef.ID3v2Tag()->frameListMap()["TPE2"].front()->toString();
                    info->setMetaData(Qmmp::ALBUMARTIST,
                                      codec->toUnicode(albumArtist.toCString(utf)).trimmed());
                }
                if(!fileRef.ID3v2Tag()->frameListMap()["TCOM"].isEmpty())
                {
                    TagLib::String composer;
                    composer = fileRef.ID3v2Tag()->frameListMap()["TCOM"].front()->toString();
                    info->setMetaData(Qmmp::COMPOSER,
                                      codec->toUnicode(composer.toCString(utf)).trimmed());
                }
                if(!fileRef.ID3v2Tag()->frameListMap()["TPOS"].isEmpty())
                {
                    TagLib::String disc = fileRef.ID3v2Tag()->frameListMap()["TPOS"].front()->toString();
                    info->setMetaData(Qmmp::DISCNUMBER, QString(disc.toCString()).trimmed());
                }
            }
            else if(tag == fileRef.APETag())
            {
                TagLib::APE::Item fld;
                if(!(fld = fileRef.APETag()->itemListMap()["ALBUM ARTIST"]).isEmpty())
                    info->setMetaData(Qmmp::ALBUMARTIST,
                                      QString::fromUtf8(fld.toString().toCString(true)).trimmed());
                if(!(fld = fileRef.APETag()->itemListMap()["COMPOSER"]).isEmpty())
                    info->setMetaData(Qmmp::COMPOSER,
                                      QString::fromUtf8(fld.toString().toCString(true)).trimmed());
            }
        }
    }
    if (fileRef.audioProperties())
        info->setLength(fileRef.audioProperties()->length());
    QList <FileInfo*> list;
    list << info;
    return list;
}

MetaDataModel* DecoderMADFactory::createMetaDataModel(const QString &path, QObject *parent)
{
   return new MPEGMetaDataModel(m_using_rusxmms, path, parent);
}
