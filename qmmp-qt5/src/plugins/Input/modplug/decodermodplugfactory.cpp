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

#include <QSettings>
#include <QFile>
#include <QStringList>
#include <QRegExp>
#include <libmodplug/stdafx.h>
#include <libmodplug/it_defs.h>
#include <libmodplug/sndfile.h>
#include "modplugmetadatamodel.h"
#include "decoder_modplug.h"
#include "archivereader.h"
#include "decodermodplugfactory.h"

// DecoderModPlugFactory

bool DecoderModPlugFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderModPlugFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("ModPlug Plugin");
    properties.filters << "*.amf" << "*.ams" << "*.dbm" << "*.dbf" << "*.dsm" << "*.far" << "*.mdl";
    properties.filters << "*.stm" << "*.ult" << "*.j2b" << "*.mt2" << "*.mdz" << "*.mdr" << "*.mdgz";
    properties.filters << "*.mdbz" << "*.mod" << "*.s3z" << "*.s3r" << "*.s3gz" << "*.s3m" << "*.xmz";
    properties.filters << "*.xmr" << "*.xmgz" << "*.itz" << "*.itr" << "*.itgz" << "*.dmf" "*.umx";
    properties.filters << "*.it" << "*.669" << "*.xm" << "*.mtm" << "*.psm" << "*.ft2" << "*.med";
    properties.description = tr("ModPlug Files");
    properties.shortName = "modplug";
    properties.hasSettings = true;
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderModPlugFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderModPlug(path);
}

QList<TrackInfo *> DecoderModPlugFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    QList <TrackInfo*> list;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    bool useFileName = settings.value("UseFileName", false).toBool();

    QByteArray buffer;

    ArchiveReader reader(nullptr);
    if(reader.isSupported(path))
    {
        buffer = reader.unpack(path);
    }
    else
    {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly))
        {
            qWarning("DecoderModPlugFactory: error: %s", qPrintable(file.errorString()));
            return list;
        }
        buffer = file.readAll();
        file.close();
    }

    if(!buffer.isEmpty())
    {
        CSoundFile *soundFile = new CSoundFile();
        soundFile->Create((uchar*) buffer.data(), buffer.size() + 1);
        TrackInfo *info = new TrackInfo(path);
        info->setDuration((qint64)soundFile->GetSongTime() * 1000);

        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TITLE, useFileName ? path.section('/',-1) :
                                                      QString::fromUtf8(soundFile->GetTitle()));
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::FORMAT_NAME, ModPlugMetaDataModel::getTypeName(soundFile->GetType()));
        }

        list << info;
        soundFile->Destroy();
        delete soundFile;
    }
    return list;
}

MetaDataModel* DecoderModPlugFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new ModPlugMetaDataModel(path);
}
