/***************************************************************************
 *   Copyright (C) 2013-2016 by Ilya Kotov                                 *
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
#include <QRegExp>
#include <QSettings>
#include <QFile>
#include <sidplayfp/SidTune.h>
#include <sidplayfp/SidTuneInfo.h>
#include "decoder_sid.h"
#include "sidhelper.h"
#include "decodersidfactory.h"

// DecoderSIDFactory

DecoderSIDFactory::DecoderSIDFactory()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");
    if(settings.value("use_hvsc", false).toBool())
    {
        QString default_path = Qmmp::configDir() + "Songlengths.txt";
        if(!m_db.open(qPrintable(settings.value("hvsc_path", default_path).toString())))
            qWarning("DecoderSIDFactory: %s", m_db.error());
    }
    settings.endGroup();
}

bool DecoderSIDFactory::supports(const QString &source) const
{
    if(source.endsWith(".mus", Qt::CaseInsensitive))
    {
        QFile file(source);
        file.open(QIODevice::ReadOnly);
        return canDecode(&file);
    }

    foreach(QString filter, properties().filters)
    {
        QRegExp regexp(filter, Qt::CaseInsensitive, QRegExp::Wildcard);
        if (regexp.exactMatch(source))
            return true;
    }
    return false;
}

bool DecoderSIDFactory::canDecode(QIODevice *input) const
{
    char buf[4];
    if (input->peek(buf, 4) != 4)
        return false;
    return (!memcmp(buf, "RSID", 4) || !memcmp(buf, "PSID", 4));
}

const DecoderProperties DecoderSIDFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("SID Plugin");
    properties.filters << "*.sid" << "*.mus" << "*.str" << "*.prg" << "*.P00" << "*.c64";
    properties.description = tr("SID Files");
    //properties.contentType = ;
    properties.shortName = "sid";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.noInput = true;
    properties.protocols << "sid";
    return properties;
}

Decoder *DecoderSIDFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderSID(&m_db, path);
}

QList<FileInfo *> DecoderSIDFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *)
{
    SIDHelper helper(&m_db);
    helper.load(fileName);
    QList <FileInfo*> list = helper.createPlayList(useMetaData);
    if(list.isEmpty())
        return list;
    if(fileName.contains("://")) //is it url?
    {
        int track = fileName.section("#", -1).toInt();
        if(track > list.count() || track < 1)
        {
            qDeleteAll(list);
            list.clear();
            return list;
        }
        FileInfo *info = list.takeAt(track - 1);
        qDeleteAll(list);
        return QList<FileInfo *>() << info;
    }
    return list;
}

MetaDataModel* DecoderSIDFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    Q_UNUSED(path);
    Q_UNUSED(parent);
    return 0;
}

Q_EXPORT_PLUGIN2(sid,DecoderSIDFactory)
