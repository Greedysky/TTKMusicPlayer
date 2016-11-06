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

#include <QtPlugin>
#include "wavpackmetadatamodel.h"
#include "decoder_wavpack.h"
#include "decoderwavpackfactory.h"
#include "cueparser.h"

// DecoderWavPackFactory

bool DecoderWavPackFactory::supports(const QString &source) const
{
    return source.endsWith(".wv", Qt::CaseInsensitive);
}

bool DecoderWavPackFactory::canDecode(QIODevice *input) const
{
    char buf[4];
    return (input->peek(buf, 4) == 4 && !memcmp(buf, "wvpk", 4));
}

const DecoderProperties DecoderWavPackFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("WavPack Plugin");
    properties.filters << "*.wv";
    properties.description = tr("WavPack Files");
    //properties.contentType = ;
    properties.shortName = "wavpack";
    properties.hasAbout = true;
    properties.hasSettings = false;
    properties.noInput = true;
    properties.protocols << "file" << "wvpack";
    return properties;
}

Decoder *DecoderWavPackFactory::create(const QString &p, QIODevice *)
{
    return new DecoderWavPack(p);
}

QList<FileInfo *> DecoderWavPackFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *ignoredFiles)
{
    QList <FileInfo*> list;
    char err[80];
    int cue_len=0;
    FileInfo *info;
    //extract metadata of one cue track
    if(fileName.contains("://"))
    {
        QString path = fileName;
        path.remove("wvpack://");
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

#if defined(Q_OS_WIN) && defined(OPEN_FILE_UTF8)
    WavpackContext *ctx = WavpackOpenFileInput (fileName.toUtf8().constData(),
                                                err, OPEN_WVC | OPEN_TAGS | OPEN_FILE_UTF8, 0);
#else
    WavpackContext *ctx = WavpackOpenFileInput (fileName.toLocal8Bit().constData(),
                                                err, OPEN_WVC | OPEN_TAGS, 0);
#endif
    if (!ctx)
    {
        qWarning("DecoderWavPackFactory: error: %s", err);
        return list;
    }
    info = new FileInfo(fileName);
    if (useMetaData)
    {
        cue_len = WavpackGetTagItem (ctx, "cuesheet", NULL, 0);
        char *value;
        if (cue_len)
        {
            value = (char*)malloc (cue_len * 2 + 1);
            WavpackGetTagItem (ctx, "cuesheet", value, cue_len + 1);
            CUEParser parser(value, fileName);
            list = parser.createPlayList();
        }
        else
        {

            char value[200];
            memset(value,0,sizeof(value));
            WavpackGetTagItem (ctx, "Album", value, sizeof(value));
            info->setMetaData(Qmmp::ALBUM, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Artist", value, sizeof(value));
            info->setMetaData(Qmmp::ARTIST, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Album Artist", value, sizeof(value));
            info->setMetaData(Qmmp::ALBUMARTIST, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Comment", value, sizeof(value));
            info->setMetaData(Qmmp::COMMENT, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Genre", value, sizeof(value));
            info->setMetaData(Qmmp::GENRE, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Title", value, sizeof(value));
            info->setMetaData(Qmmp::TITLE, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Composer", value, sizeof(value));
            info->setMetaData(Qmmp::COMPOSER, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Year", value, sizeof(value));
            info->setMetaData(Qmmp::YEAR, QString::fromUtf8(value).toInt());
            WavpackGetTagItem (ctx, "Track", value, sizeof(value));
            info->setMetaData(Qmmp::TRACK, QString::fromUtf8(value).toInt());
            WavpackGetTagItem (ctx, "Disc", value, sizeof(value));
            info->setMetaData(Qmmp::DISCNUMBER, QString::fromUtf8(value).toInt());
        }
    }

    if (cue_len==0)
    {
        info->setLength((int) WavpackGetNumSamples(ctx)/WavpackGetSampleRate(ctx));
        list << info;
    }
    WavpackCloseFile (ctx);
    return list;
}

MetaDataModel* DecoderWavPackFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    if (!path.contains("://") || path.startsWith("wvpack://"))
        return new WavPackMetaDataModel(path, parent);
    else
        return 0;
}

Q_EXPORT_PLUGIN2(wavpack,DecoderWavPackFactory)
