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

#include "decoderadplugfactory.h"
#include "adplugwrap.h"
#include "decoder_adplug.h"
#include "magic.h"
#include "adplugmetadatamodel.h"

class InputStreamQIO : public InputStream
{
public:
    explicit InputStreamQIO(QIODevice *device)
        : device(device)
    {

    }

    virtual bool read(std::size_t offset, void *buf, std::size_t n) override
    {
        qint64 ret;
        if(!device->seek(offset))
        {
            return false;
        }

        ret = device->read(reinterpret_cast<char *>(buf), n);
        return ret > 0 && ret == (qint64)n;
    }

    virtual size_t size() override
    {
        if(device->isSequential() || !device->isOpen())
        {
            throw InputStream::NoRandomAccess();
        }
        return device->size();
    }

private:
    QIODevice *device;

};

bool DecoderAdplugFactory::canDecode(QIODevice *device) const
{
    return adplug_supports(InputStreamQIO(device));
}

DecoderProperties DecoderAdplugFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("AdPlug Plugin");
    properties.filters << "*.adl" << "*.hsc" << "*.ksm" << "*.lds";
    properties.description = tr("AdPlug Files");
    properties.shortName = "adplug";
    properties.hasSettings = false;
    return properties;
}

Decoder *DecoderAdplugFactory::create(const QString &path, QIODevice *)
{
    return new DecoderAdplug(path);
}

QList<TrackInfo *> DecoderAdplugFactory::createPlayList(const QString &filename, TrackInfo::Parts parts, QStringList *)
{
    QList<TrackInfo *> list;
    if(parts & TrackInfo::Properties)
    {
        try
        {
            AdplugWrap adplug(filename.toUtf8().constData());
            TrackInfo *file_info = new TrackInfo(filename);
            file_info->setDuration(adplug.length() / 1000);
            list << file_info;
        }
        catch(const AdplugWrap::InvalidFile &)
        {
        }
    }
    return list;
}

MetaDataModel *DecoderAdplugFactory::createMetaDataModel(const QString &path, bool)
{
    return new AdplugMetaDataModel(path);
}
