#include "decoderxmpfactory.h"
#include "decoder_xmp.h"
#include "xmpmetadatamodel.h"
#include "settingsdialog.h"

#include <QFileInfo>

bool DecoderXMPFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    xmp_context ctx = xmp_create_context();
    const bool v = file ? xmp_load_module(ctx, QmmpPrintable(file->fileName())) == 0 : false;
    if(v)
    {
      xmp_release_module(ctx);
    }

    xmp_free_context(ctx);
    return v;
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
    properties.filters << "*.xmf";
    properties.description = "XMP Module File";
    properties.protocols << "file";
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

    if(parts & TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, mi.mod->name);
        info->setValue(Qmmp::COMMENT, mi.comment);
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, QFileInfo(path).size() * 8.0 / mi.seq_data[0].duration + 1.0f);
        info->setValue(Qmmp::SAMPLERATE, 44100);
        info->setValue(Qmmp::CHANNELS, 2);
        info->setValue(Qmmp::BITS_PER_SAMPLE, 16);
        info->setValue(Qmmp::FORMAT_NAME, mi.mod->type);
        info->setDuration(mi.seq_data[0].duration);
    }

    xmp_release_module(ctx);
    xmp_free_context(ctx);
    return plyalist << info;
}

MetaDataModel* DecoderXMPFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new XMPMetaDataModel(path);
}

QDialog *DecoderXMPFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(xmp, DecoderXMPFactory)
#endif
