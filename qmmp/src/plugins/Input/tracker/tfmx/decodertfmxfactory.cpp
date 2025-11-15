#include "decodertfmxfactory.h"
#include "tfmxhelper.h"
#include "decoder_tfmx.h"
#include "settingsdialog.h"

bool DecoderTFMXFactory::canDecode(QIODevice *input) const
{
    // At least 0xb80 is needed for some modules that start with machine code player.
    constexpr int peekSize = 0xb80;
    char buf[peekSize];
    if(input->peek(buf, peekSize) != peekSize)
    {
        return false;
    }

    void *ctx = tfmxdec_new();
    const int v = tfmxdec_detect(ctx, buf, peekSize);
    tfmxdec_delete(ctx);
    return v;
}

DecoderProperties DecoderTFMXFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("TFMX Plugin");
    properties.shortName = "tfmx";
    properties.filters << TFMXHelper::filters();
    properties.description = "TFMX related Audio File";
    properties.protocols << "file" << "tfmx";
	properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderTFMXFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderTFMX(path);
}

QList<TrackInfo*> DecoderTFMXFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
{
    if(path.contains("://")) //is it one track?
    {
        int track = -1;
        const QString &filePath = TrackInfo::pathFromUrl(path, &track);

        QList<TrackInfo*> playlist = createPlayList(filePath, parts, ignoredPaths);
        if(playlist.isEmpty() || track <= 0 || track > playlist.count())
        {
            qDeleteAll(playlist);
            playlist.clear();
            return playlist;
        }

        TrackInfo *info = playlist.takeAt(track - 1);
        qDeleteAll(playlist);
        playlist.clear();
        return playlist << info;
    }

    TFMXHelper helper(path);
    if(!helper.initialize())
    {
        qWarning("DecoderTFMXFactory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderTFMXFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderTFMXFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(tfmx, DecoderTFMXFactory)
#endif
