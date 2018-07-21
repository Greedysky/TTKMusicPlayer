/***************************************************************************
 *   Copyright (C) 2007-2018 by Ilya Kotov                                 *
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
#ifdef Q_OS_WIN
#include <windows.h>
#define ENABLE_SNDFILE_WINDOWS_PROTOTYPES 1
#endif
#include <sndfile.h>
#include "decoder_sndfile.h"
#include "decodersndfilefactory.h"

#define WAVE_FORMAT_PCM 0x0001
#define WAVE_FORMAT_ADPCM 0x0002
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#define WAVE_FORMAT_ALAW 0x0006
#define WAVE_FORMAT_MULAW 0x0007

// DecoderSndFileFactory
bool DecoderSndFileFactory::canDecode(QIODevice *input) const
{
    char buf[36];
    if(input->peek(buf, sizeof(buf)) != sizeof(buf))
        return false;

    if(!memcmp(buf + 8, "WAVE", 4) && (!memcmp(buf, "RIFF", 4) || !memcmp(buf, "RIFX", 4)))
    {
        quint16 subformat = (quint16(buf[21]) << 8) + buf[20];

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

const DecoderProperties DecoderSndFileFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Sndfile Plugin");
    properties.filters << "*.wav" << "*.au" << "*.snd" << "*.aif" << "*.aiff" << "*.8svx";
    properties.filters << "*.sph" << "*.sf" << "*.voc" << "*.w64";
    properties.description = tr("PCM Files");
    //properties.contentType = "";
    properties.shortName = "sndfile";
    properties.hasAbout = true;
    properties.hasSettings = false;
    properties.noInput = false;
    return properties;
}

Decoder *DecoderSndFileFactory::create(const QString &, QIODevice *input)
{
    return new DecoderSndFile(input);
}

QList<FileInfo *> DecoderSndFileFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *)
{
    QList <FileInfo *> list;
    SF_INFO snd_info;
    SNDFILE *sndfile = 0;
    memset (&snd_info, 0, sizeof(snd_info));
    snd_info.format = 0;
#ifdef Q_OS_WIN
        sndfile = sf_wchar_open(reinterpret_cast<LPCWSTR>(fileName.utf16()), SFM_READ, &snd_info);
#else
        sndfile = sf_open(fileName.toLocal8Bit().constData(), SFM_READ, &snd_info);
#endif
    if (!sndfile)
        return list;

    list << new FileInfo(fileName);
    if (useMetaData)
    {
        const char *title = sf_get_string(sndfile, SF_STR_TITLE);
        if(title)
            list.at(0)->setMetaData(Qmmp::TITLE, QString::fromUtf8(title).trimmed());

        const char *artist = sf_get_string(sndfile, SF_STR_ARTIST);
        if(artist)
            list.at(0)->setMetaData(Qmmp::ARTIST, QString::fromUtf8(artist).trimmed());

        const char *comment = sf_get_string(sndfile, SF_STR_COMMENT);
        if(comment)
            list.at(0)->setMetaData(Qmmp::COMMENT, QString::fromUtf8(comment).trimmed());
    }

    list.at(0)->setLength(int(snd_info.frames / snd_info.samplerate));

    sf_close(sndfile);
    return list;
}

MetaDataModel* DecoderSndFileFactory::createMetaDataModel(const QString&, QObject *)
{
    return 0;
}

Q_EXPORT_PLUGIN2(sndfile, DecoderSndFileFactory)
