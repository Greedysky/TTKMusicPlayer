/***************************************************************************
 *   Copyright (C) 2007-2019 by Ilya Kotov                                 *
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

#include <QRegExp>
#include <QtPlugin>
#include <QFileInfo>
#ifdef Q_OS_WIN
#include <windows.h>
#define ENABLE_SNDFILE_WINDOWS_PROTOTYPES 1
#endif
#include <sndfile.h>
#include "decoder_sndfile.h"
#include "decodersndfilefactory.h"

#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM 0x0001
#endif
#define WAVE_FORMAT_ADPCM 0x0002
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#define WAVE_FORMAT_ALAW 0x0006
#define WAVE_FORMAT_MULAW 0x0007

// DecoderSndFileFactory
bool DecoderSndFileFactory::canDecode(QIODevice *input) const
{
    char buf[36] = {0};
    if(input->peek(buf, sizeof(buf)) != sizeof(buf))
        return false;

    if(!memcmp(buf + 8, "WAVE", 4) && (!memcmp(buf, "RIFF", 4) || !memcmp(buf, "RIFX", 4)))
    {
        quint16 subformat = 0;

        if(!memcmp(buf + 12, "fmt ", 4))
        {
            subformat = (quint16(buf[21]) << 8) + buf[20];
        }
        else if(!input->isSequential())
        {
            input->seek(12);
            //skip "JUNK" and "bext" chunks
            while(!input->atEnd())
            {
                if(input->peek(buf, sizeof(buf)) != sizeof(buf))
                    return false;

                if(!memcmp(buf, "fmt ", 4))
                {
                    subformat = (quint16(buf[9]) << 8) + buf[8];
                    break;
                }
                else if(!memcmp(buf, "JUNK", 4) || !memcmp(buf, "bext", 4))
                {
                    size_t size = buf[4] + (buf[5] << 8) + (buf[6] << 16) + (buf[7] << 24);
                    if(!input->seek(input->pos() + size + 8))
                        break;
                }
                else
                {
                    break;
                }
            }
            input->seek(0);
        }

        switch (subformat)
        {
        case WAVE_FORMAT_PCM:
        case WAVE_FORMAT_ADPCM:
        case WAVE_FORMAT_IEEE_FLOAT:
        case WAVE_FORMAT_ALAW:
        case WAVE_FORMAT_MULAW:
            return true;
        default:
            return false;
        }
    }
    else if(!memcmp(buf, "FORM", 4))
    {
        if(!memcmp(buf + 8, "AIFF", 4))
            return true;
        if(!memcmp(buf + 8, "8SVX", 4))
            return true;
    }
    else if(!memcmp(buf, ".snd", 4) || !memcmp(buf, "dns.", 4))
        return true;
    else if(!memcmp(buf, "fap ", 4) || !memcmp(buf, " paf", 4))
        return true;
    else if(!memcmp(buf, "NIST", 4))
        return true;
    else if(!memcmp(buf, "Crea", 4) && !memcmp(buf + 4, "tive", 4))
        return true;
    else if(!memcmp(buf, "riff", 4))
        return true;

    return false;
}

DecoderProperties DecoderSndFileFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Sndfile Plugin");
    properties.filters << "*.wav" << "*.au" << "*.snd" << "*.aif" << "*.aiff" << "*.8svx";
    properties.filters << "*.sph" << "*.sf" << "*.voc" << "*.w64";
    properties.description = tr("PCM Files");
    properties.shortName = "sndfile";
    properties.hasSettings = false;
    properties.noInput = false;
    return properties;
}

Decoder *DecoderSndFileFactory::create(const QString &, QIODevice *input)
{
    return new DecoderSndFile(input);
}

QList<TrackInfo *> DecoderSndFileFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
        return QList<TrackInfo *>() << info;

    SF_INFO snd_info;
    SNDFILE *sndfile = 0;
    memset(&snd_info, 0, sizeof(snd_info));
    snd_info.format = 0;
#ifdef Q_OS_WIN
        sndfile = sf_wchar_open(reinterpret_cast<LPCWSTR>(path.utf16()), SFM_READ, &snd_info);
#else
        sndfile = sf_open(path.toLocal8Bit().constData(), SFM_READ, &snd_info);
#endif
    if(!sndfile)
    {
        delete info;
        return QList<TrackInfo *>();
    }

    if(parts & TrackInfo::MetaData)
    {
        const char *title = sf_get_string(sndfile, SF_STR_TITLE);
        info->setValue(Qmmp::TITLE, title ? QString::fromUtf8(title) : QString());
        const char *date = sf_get_string(sndfile, SF_STR_DATE);
        info->setValue(Qmmp::YEAR, date ? QString::fromUtf8(date) : QString());
        const char *album = sf_get_string(sndfile, SF_STR_ALBUM);
        info->setValue(Qmmp::ALBUM, album ? QString::fromUtf8(album) : QString());
        const char *track = sf_get_string(sndfile, SF_STR_TRACKNUMBER);
        info->setValue(Qmmp::TRACK, track ? QString::fromUtf8(track) : QString());
        const char *artist = sf_get_string(sndfile, SF_STR_ARTIST);
        info->setValue(Qmmp::ARTIST, artist ? QString::fromUtf8(artist) : QString());
        const char *comment = sf_get_string(sndfile, SF_STR_COMMENT);
        info->setValue(Qmmp::COMMENT, comment ? QString::fromUtf8(comment) : QString());
        const char *genre = sf_get_string(sndfile, SF_STR_GENRE);
        info->setValue(Qmmp::COMMENT, genre ? QString::fromUtf8(genre) : QString());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, QFileInfo(path).size() * 8000.0 / info->duration() + 0.5);
        info->setValue(Qmmp::SAMPLERATE, snd_info.samplerate);
        info->setValue(Qmmp::CHANNELS, snd_info.channels);
        switch(snd_info.format & SF_FORMAT_SUBMASK)
        {
        case SF_FORMAT_PCM_S8:
        case SF_FORMAT_PCM_U8:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 8);
            break;
        case SF_FORMAT_PCM_16:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 16);
            break;
        case SF_FORMAT_PCM_24:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 24);
            break;
        case SF_FORMAT_PCM_32:
        case SF_FORMAT_FLOAT:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 32);
            break;
        case SF_FORMAT_DOUBLE:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 64);
            break;
        }
        SF_FORMAT_INFO format_info;
        memset(&format_info, 0, sizeof(format_info));
        format_info.format = (snd_info.format & SF_FORMAT_TYPEMASK);
        sf_command(0, SFC_GET_FORMAT_INFO, &format_info, sizeof(format_info));
        info->setValue(Qmmp::FORMAT_NAME, QString::fromLatin1(format_info.name));
        info->setDuration(int(snd_info.frames * 1000 / snd_info.samplerate));
    }

    sf_close(sndfile);
    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderSndFileFactory::createMetaDataModel(const QString&, bool)
{
    return 0;
}

Q_EXPORT_PLUGIN2(sndfile, DecoderSndFileFactory)
