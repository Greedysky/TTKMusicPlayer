#include "decodertfmxfactory.h"
#include "tfmxhelper.h"
#include "decoder_tfmx.h"

bool DecoderTFMXFactory::canDecode(QIODevice *input) const
{
    char buf[9];
    if(input->peek(buf, 9) != 9)
        return false;
    return !memcmp(buf, "TFMXSONG", 8) || !memcmp(buf, "TFMX-MOD", 8) || !memcmp(buf, "TFMX ", 5) ||
           !memcmp(buf, "TFMX-SONG", 9) || !memcmp(buf, "TFMX_SONG", 9);
}

DecoderProperties DecoderTFMXFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("TFMX Plugin");
    properties.shortName = "tfmx";
    properties.filters << TFMXHelper::filters();
    properties.description = "Final Music System Tracker Module File";
    properties.protocols << "file" << "tfmx";
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
    else
    {
        if(ignoredPaths)
        {
            ignoredPaths->push_back(path);
        }
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
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(tfmx, DecoderTFMXFactory)
#endif
