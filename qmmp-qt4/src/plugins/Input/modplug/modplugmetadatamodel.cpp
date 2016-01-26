/***************************************************************************
 *   Copyright (C) 2009 by Ilya Kotov                                      *
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

#include <QFile>
#include <stdint.h>
#include <libmodplug/stdafx.h>
#include <libmodplug/it_defs.h>
#include <libmodplug/sndfile.h>
#include <libmodplug/modplug.h>
#include "archivereader.h"
#include "modplugmetadatamodel.h"

#define MAX_MESSAGE_LENGTH 4000

ModPlugMetaDataModel::ModPlugMetaDataModel(const QString &path, QObject *parent) : MetaDataModel(parent)
{
    m_soundFile = 0;
    m_path = path;
    ArchiveReader reader(this);
    if(reader.isSupported(m_path))
    {
        m_buffer = reader.unpack(m_path);
    }
    else
    {
        QFile file(m_path);
        if(!file.open(QIODevice::ReadOnly))
        {
            qWarning("DetailsDialog: error: %s", qPrintable(file.errorString ()));
            return;
        }
        m_buffer = file.readAll();
        file.close();
    }
    m_soundFile = new CSoundFile();
    m_soundFile->Create((uchar*) m_buffer.data(), m_buffer.size());
}

ModPlugMetaDataModel::~ModPlugMetaDataModel()
{
    if(m_soundFile)
    {
        m_soundFile->Destroy();
        delete m_soundFile;
    }
}

QHash<QString, QString> ModPlugMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    if(!m_soundFile)
        return ap;

    ap.insert(tr("File name"), m_path.section('/',-1));
    QString text;
    switch(m_soundFile->GetType())
    {
    case MOD_TYPE_MOD:
        text += "ProTracker";
        break;
    case MOD_TYPE_S3M:
        text += "Scream Tracker 3";
        break;
    case MOD_TYPE_XM:
        text += "Fast Tracker 2";
        break;
    case MOD_TYPE_IT:
        text += "Impulse Tracker";
        break;
    case MOD_TYPE_MED:
        text += "OctaMed";
        break;
    case MOD_TYPE_MTM:
        text += "MTM";
        break;
    case MOD_TYPE_669:
        text += "669 Composer / UNIS 669";
        break;
    case MOD_TYPE_ULT:
        text += "ULT";
        break;
    case MOD_TYPE_STM:
        text += "Scream Tracker";
        break;
    case MOD_TYPE_FAR:
        text += "Farandole";
        break;
    case MOD_TYPE_AMF:
        text += "ASYLUM Music Format";
        break;
    case MOD_TYPE_AMS:
        text += "AMS module";
        break;
    case MOD_TYPE_DSM:
        text += "DSIK Internal Format";
        break;
    case MOD_TYPE_MDL:
        text += "DigiTracker";
        break;
    case MOD_TYPE_OKT:
        text += "Oktalyzer";
        break;
    case MOD_TYPE_DMF:
        text += "Delusion Digital Music Fileformat (X-Tracker)";
        break;
    case MOD_TYPE_PTM:
        text += "PolyTracker";
        break;
    case MOD_TYPE_DBM:
        text += "DigiBooster Pro";
        break;
    case MOD_TYPE_MT2:
        text += "MT2";
        break;
    case MOD_TYPE_AMF0:
        text += "AMF0";
        break;
    case MOD_TYPE_PSM:
        text += "PSM";
        break;
    default:
        text += "Unknown";
        break;
    }
    ap.insert(tr("Type"), text);

    int lSongTime = m_soundFile->GetSongTime();
    text = QString("%1").arg(lSongTime/60);
    text +=":"+QString("%1").arg(lSongTime%60,2,10,QChar('0'));
    ap.insert(tr("Length"), text);
    ap.insert(tr("Speed"), QString::number(m_soundFile->GetMusicSpeed()));
    ap.insert(tr("Tempo"), QString::number(m_soundFile->GetMusicTempo()));
    ap.insert(tr("Samples"), QString::number(m_soundFile->GetNumSamples()));
    ap.insert(tr("Instruments"), QString::number(m_soundFile->GetNumInstruments()));
    ap.insert(tr("Patterns"), QString::number(m_soundFile->GetNumPatterns()));
    ap.insert(tr("Channels"), QString::number(m_soundFile->GetNumChannels()));
    return ap;
}

QHash<QString, QString> ModPlugMetaDataModel::descriptions()
{
    QHash<QString, QString> desc;
    if(!m_soundFile)
        return desc;
    char lBuffer[33];
    QString text;
    for(uint i = 0; i < m_soundFile->GetNumSamples(); i++)
    {
        m_soundFile->GetSampleName(i, lBuffer);
        text += QString::fromUtf8(lBuffer) + '\n';
    }
    text = text.trimmed();
    if(!text.isEmpty())
        desc.insert(tr("Samples"), text);
    text.clear();
    for(uint i = 0; i < m_soundFile->GetNumInstruments(); i++)
    {
        m_soundFile->GetInstrumentName(i, lBuffer);
        text += QString::fromUtf8(lBuffer) + '\n';
    }
    text = text.trimmed();
    if(!text.isEmpty())
        desc.insert(tr("Instruments"), text);
    text.clear();
    char message[MAX_MESSAGE_LENGTH];
    int length = m_soundFile->GetSongComments(message, MAX_MESSAGE_LENGTH, 80);
    if (length != 0)
        desc.insert(tr("Comment"), QString::fromUtf8(message).trimmed ());
    return desc;
}
