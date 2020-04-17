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
#include "adplughelper.h"
#include "decoder_adplug.h"
#include "magic.h"
#include "adplugmetadatamodel.h"

class InputStreamQIO : public InputStream
{
public:
    explicit InputStreamQIO(QIODevice *i)
        : device(i)
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

bool DecoderAdplugFactory::canDecode(QIODevice *input) const
{
    return adplug_supports(InputStreamQIO(input));
}

DecoderProperties DecoderAdplugFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("AdPlug Plugin");
    properties.filters << "*.adl" << "*.hsc" << "*.ksm" << "*.lds";
    properties.description = tr("AdLib Sound Files");
    properties.shortName = "adplug";
    return properties;
}

Decoder *DecoderAdplugFactory::create(const QString &path, QIODevice *)
{
    return new DecoderAdplug(path);
}

QList<TrackInfo *> DecoderAdplugFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    AdplugHelper helper(path.toUtf8().constData());
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo *>();
    }

    if(parts & TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, QString::fromStdString(helper.title()));
        info->setValue(Qmmp::ARTIST, QString::fromStdString(helper.author()));
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::FORMAT_NAME, QString::fromStdString(helper.format()));
        info->setDuration(helper.length());
    }

    return QList<TrackInfo *>() << info;
}

MetaDataModel *DecoderAdplugFactory::createMetaDataModel(const QString &path, bool)
{
    return new AdplugMetaDataModel(path);
}
