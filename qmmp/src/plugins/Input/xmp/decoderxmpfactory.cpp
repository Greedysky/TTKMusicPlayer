#include "decoderxmpfactory.h"
#include "decoder_xmp.h"
#include "xmpmetadatamodel.h"
#include "settingsdialog.h"

#include <QFileInfo>

bool DecoderXMPFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderXMPFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("XMP Plugin");
    properties.shortName = "xmp";
    properties.filters << "*.abk";
    properties.filters << "*.dsym";
    properties.filters << "*.emod";
    properties.filters << "*.fnk";
    properties.filters << "*.gtk";
    properties.filters << "*.ims";
    properties.filters << "*.liq";
    properties.filters << "*.mfp" << "*.mgt";
    properties.filters << "*.rtm";
    properties.filters << "*.tcb";
    properties.description = "XMP Module File";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderXMPFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderXMP(path);
}

QList<TrackInfo*> DecoderXMPFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    QList<TrackInfo*> plyalist;
    TrackInfo *info = new TrackInfo(path);

    if(parts & (TrackInfo::MetaData | TrackInfo::Properties))
    {
        xmp_context ctx = xmp_create_context();
        if(xmp_load_module(ctx, QmmpPrintable(path)) != 0)
        {
            qWarning("DecoderXMPFactory: unable to load module");
            xmp_free_context(ctx);
            delete info;
            return plyalist;
        }

        xmp_module_info mi;
        xmp_get_module_info(ctx, &mi);

        info->setValue(Qmmp::TITLE, mi.mod->name);
        info->setValue(Qmmp::BITRATE, QFileInfo(path).size() * 8.0 / mi.seq_data[0].duration + 1.0f);
        info->setValue(Qmmp::SAMPLERATE, 44100);
        info->setValue(Qmmp::CHANNELS, 2);
        info->setValue(Qmmp::BITS_PER_SAMPLE, 16);
        info->setValue(Qmmp::FORMAT_NAME, mi.mod->type);
        info->setDuration(mi.seq_data[0].duration);

        xmp_release_module(ctx);
        xmp_free_context(ctx);
    };
    return plyalist << info;
}

MetaDataModel* DecoderXMPFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new XMPMetaDataModel(path);
}

void DecoderXMPFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(xmp, DecoderXMPFactory)
#endif
