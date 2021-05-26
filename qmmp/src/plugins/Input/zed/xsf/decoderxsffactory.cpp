#include "xsfhelper.h"
#include "decoder_xsf.h"
#include "decoderxsffactory.h"

bool DecoderXSFFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderXSFFactory::properties() const
{
    DecoderProperties properties;
    properties.name = "XSF Plugin";
    properties.shortName = "xsf";
    properties.filters << "*.2sf" << "*.mini2sf";
    properties.filters << "*.gsf" << "*.minigsf";
    properties.filters << "*.usf" << "*.miniusf";
    properties.filters << "*.ncsf" << "*.minincsf";
    properties.filters << "*.snsf" << "*.minisnsf";
    properties.description = "Overload Audio File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderXSFFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderXSF(path);
}

QList<TrackInfo*> DecoderXSFFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    XSFHelper helper(path);
    helper.metaOnly(true);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        const QMap<Qmmp::MetaData, QString> metaData(helper.readMetaData());
        for(auto itr = metaData.begin(); itr != metaData.end(); ++itr)
        {
            info->setValue(itr.key(), itr.value());
        }
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "XSF");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderXSFFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderXSFFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(xsf, DecoderXSFFactory)
#endif
