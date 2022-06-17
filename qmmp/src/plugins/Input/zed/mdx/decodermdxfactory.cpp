#include "decodermdxfactory.h"
#include "mdxhelper.h"
#include "decoder_mdx.h"

bool DecoderMDXFactory::canDecode(QIODevice *input) const
{
    QFile *file = static_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    MDXHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderMDXFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("MDX Plugin");
    properties.shortName = "mdx";
    properties.filters << "*.mdx" << "*.pdx" << "*.m" << "*.mub" << "*.muc" << "*.vgs" << "*.bgm" << "*.mml";
    properties.description = "MDX Game Audio File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderMDXFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderMDX(path);
}

QList<TrackInfo*> DecoderMDXFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    MDXHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::ARTIST, helper.author());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "MDX");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderMDXFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderMDXFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(mdx, DecoderMDXFactory)
#endif
