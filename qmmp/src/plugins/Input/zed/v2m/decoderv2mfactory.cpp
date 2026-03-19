#include "decoderv2mfactory.h"
#include "v2mhelper.h"
#include "decoder_v2m.h"
#include "archivereader.h"

bool DecoderV2MFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    V2MHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderV2MFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("V2M Plugin");
    properties.shortName = "v2m";
    properties.filters << "*.v2m";
    properties.filters << ArchiveReader::filters();
    properties.description = "V2 Module Player File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderV2MFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderV2M(path);
}

QList<TrackInfo> DecoderV2MFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo raw(path), *info = &raw;
    if(parts == TrackInfo::Parts())
    {
        return {raw};
    }

    V2MHelper helper(path);
    if(!helper.initialize())
    {
        return {};
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "V2 Module");
        info->setDuration(helper.totalTime());
    }

    return {raw};
}

MetaDataModel* DecoderV2MFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderV2MFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(v2m, DecoderV2MFactory)
#endif
