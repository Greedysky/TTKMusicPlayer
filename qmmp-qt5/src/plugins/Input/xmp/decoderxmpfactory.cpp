/***************************************************************************
 *   Copyright (C) 2015-2019 by Ilya Kotov                                 *
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

#include <QStringList>
#include <QRegExp>
#include <xmp.h>
#include "decoder_xmp.h"
#include "xmpmetadatamodel.h"
#include "decoderxmpfactory.h"

// DecodeXmpFactory

bool DecoderXmpFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderXmpFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("XMP Plugin");
    properties.filters << "*.mod" << "*.m15" << "*.nt" << "*.flx" << "*.wow";
    properties.filters << "*.dbm" << "*.digi" << "*.emod" << "*.med" << "*.mtn" << "*.okt" << "*.sfx";
    properties.filters << "*.dtm" << "*.gtk" << "*.mgt";
    properties.filters << "*.669" << "*.far" << "*.fnk" << "*.imf" << "*.it" << "*.liq" << "*.mdl";
    properties.filters << "*.mtm" << "*.rtm" << "*.s3m" << "*.stm" << "*.ult" << "*.xm";
    properties.filters << "*.amf" << "*.gdm" << "*.stx";
    properties.filters << "*.abk" << "*.amf" << "*.psm" << "*.j2b" << "*.mfp" << "*.smp" << "*.stim" << "*.umx";
    properties.filters << "*.amd" << "*.rad" << "*.hsc" << "*.s3m";
    properties.filters << "*.xm";
    properties.filters << "*.s3z" << "*.s3r" << "*.s3gz";
    properties.filters << "*.mdz" << "*.mdr" << "*.mdbz" << "*.mdgz";
    properties.filters << "*.itz" << "*.itr" << "*.itgz";
    properties.filters << "*.xmr" << "*.xmgz" << "*.xmz";
    properties.description = tr("Module Files");
    properties.shortName = "xmp";
    properties.hasSettings = true;
    properties.noInput = true;
    properties.protocols << "file";
    properties.priority = 10;
    return properties;
}

Decoder *DecoderXmpFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderXmp(path);
}

QList<TrackInfo *> DecoderXmpFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    QList <TrackInfo*> list;
    TrackInfo *info = new TrackInfo(path);
    if(parts & (TrackInfo::MetaData | TrackInfo::Properties))
    {
        xmp_context ctx = xmp_create_context();
        if(xmp_load_module(ctx, path.toLocal8Bit().data()) != 0)
        {
            qWarning("DecoderXmpFactory: unable to load module");
            xmp_free_context(ctx);
            delete info;
            return list;
        }
        xmp_module_info mi;
        xmp_get_module_info(ctx, &mi);
        info->setValue(Qmmp::TITLE, mi.mod->name);
        info->setValue(Qmmp::FORMAT_NAME, mi.mod->type);
        info->setDuration(mi.seq_data[0].duration);
        xmp_release_module(ctx);
        xmp_free_context(ctx);
    }
    list << info;
    return list;
}

MetaDataModel* DecoderXmpFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new XmpMetaDataModel(path);
}
