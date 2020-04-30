/***************************************************************************
 *   Copyright (C) 2008-2020 by Ilya Kotov                                 *
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

#include "decoder_cue.h"
#include "cuemetadatamodel.h"
#include "cuefile.h"
#include "decodercuefactory.h"

bool DecoderCUEFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderCUEFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("CUE Plugin");
    properties.shortName = "cue";
    properties.filters << "*.cue";
    properties.description = tr("Cuesheet Files");
    properties.protocols << "cue";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderCUEFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderCUE(path);
}

QList<TrackInfo*> DecoderCUEFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
{
    Q_UNUSED(parts);
    CueFile cueFile(path);
    if(path.contains("://"))
    {
        int track = path.section("#", -1).toInt();
        return cueFile.createPlayList(track);
    }
    else
    {
        ignoredPaths->append(cueFile.dataFilePaths());
        return cueFile.createPlayList();
    }
}

MetaDataModel* DecoderCUEFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return path.startsWith("cue://") ? new CUEMetaDataModel(path) : nullptr;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(cue, DecoderCUEFactory)
#endif
