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

#include <QStringList>
#include <QRegExp>
#include <QtPlugin>
#include <QSettings>
#include <QFile>
#include <libmodplug/stdafx.h>
#include <libmodplug/it_defs.h>
#include <libmodplug/sndfile.h>
#include "modplugmetadatamodel.h"
#include "decoder_modplug.h"
#include "archivereader.h"
#include "decodermodplugfactory.h"


// DecoderModPlugFactory

bool DecoderModPlugFactory::supports(const QString &source) const
{
    foreach(QString filter, properties().filters)
    {
        QRegExp regexp(filter, Qt::CaseInsensitive, QRegExp::Wildcard);
        if (regexp.exactMatch(source))
            return true;
    }
    return false;
}

bool DecoderModPlugFactory::canDecode(QIODevice *) const
{
    return false;
}

const DecoderProperties DecoderModPlugFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("ModPlug Plugin");
    properties.filters << "*.amf" << "*.ams" << "*.dbm" << "*.dbf" << "*.dsm" << "*.far" << "*.mdl";
    properties.filters << "*.stm" << "*.ult" << "*.j2b" << "*.mt2" << "*.mdz" << "*.mdr" << "*.mdgz";
    properties.filters << "*.mdbz" << "*.mod" << "*.s3z" << "*.s3r" << "*.s3gz" << "*.s3m" << "*.xmz";
    properties.filters << "*.xmr" << "*.xmgz" << "*.itz" << "*.itr" << "*.itgz" << "*.dmf" "*.umx";
    properties.filters << "*.it" << "*.669" << "*.xm" << "*.mtm" << "*.psm" << "*.ft2" << "*.med";
    properties.description = tr("ModPlug Files");
    //properties.contentType = ;
    properties.shortName = "modplug";
    properties.hasAbout = true;
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

QList<FileInfo *> DecoderModPlugFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *)
{
    QList <FileInfo*> list;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    if (!useMetaData || settings.value("UseFileName", false).toBool())
    {
        list << new FileInfo(fileName);
        list.at(0)->setMetaData(Qmmp::TITLE, fileName.section('/',-1));
        return list;
    }
    ArchiveReader reader(0);
    QByteArray buffer;
    if (reader.isSupported(fileName))
    {
        buffer = reader.unpack(fileName);
    }
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning("DecoderModPlugFactory: error: %s", qPrintable(file.errorString ()));
            return list;
        }
        buffer = file.readAll();
        file.close();
    }
    CSoundFile* soundFile = new CSoundFile();
    soundFile->Create((uchar*) buffer.data(), buffer.size()+1);
    list << new FileInfo(fileName);
    list.at(0)->setLength((int) soundFile->GetSongTime());
    list.at(0)->setMetaData(Qmmp::TITLE, QString::fromUtf8(soundFile->GetTitle()));
    soundFile->Destroy();
    delete soundFile;
    return list;
}

MetaDataModel* DecoderModPlugFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    return new ModPlugMetaDataModel(path, parent);
}

Q_EXPORT_PLUGIN2(modplug,DecoderModPlugFactory)
