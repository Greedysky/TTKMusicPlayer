#include "decoderxsffactory.h"
#include "xsfhelper.h"
#include "decoder_xsf.h"
#include "xsfmetadatamodel.h"

bool DecoderXSFFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    XSFHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderXSFFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("XSF Plugin");
    properties.shortName = "xsf";
    properties.filters << "*.2sf" << "*.mini2sf";
    properties.filters << "*.gsf" << "*.minigsf";
    properties.filters << "*.usf" << "*.miniusf";
    properties.filters << "*.ncsf" << "*.minincsf";
    properties.filters << "*.snsf" << "*.minisnsf";
    properties.filters << "*.pcm" << "*.msu";
    properties.description = "Audio Overload File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderXSFFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderXSF(path);
}

TrackInfoList DecoderXSFFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo raw(path), *info = &raw;
    if(parts == TrackInfo::Parts())
    {
        return {raw};
    }

    XSFHelper helper(path, true);
    if(!helper.initialize())
    {
        return {};
    }

    if((parts & TrackInfo::MetaData) && helper.hasTags())
    {
        info->setValue(Qmmp::TITLE, helper.tag("title"));
        info->setValue(Qmmp::ARTIST, helper.tag("artist"));
        info->setValue(Qmmp::ALBUM, helper.tag("album"));
        info->setValue(Qmmp::GENRE, helper.tag("genre"));
        info->setValue(Qmmp::YEAR, helper.tag("year"));
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, helper.format());
        info->setDuration(helper.totalTime());
    }

    return {raw};
}

MetaDataModel *DecoderXSFFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new XSFMetaDataModel(path);
}

QDialog *DecoderXSFFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(xsf, DecoderXSFFactory)
#endif
