#include "decodervgmstreamfactory.h"
#include "vgmstreamhelper.h"
#include "decoder_vgmstream.h"
#include "settingsdialog.h"

bool DecoderVgmstreamFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    VgmstreamHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderVgmstreamFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Vgmstream Plugin");
    properties.shortName = "vgmstream";
    properties.filters << VgmstreamHelper::filters();
    properties.description = "Video Game Music Steams File";
    properties.protocols << "file" << "vgm";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderVgmstreamFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderVgmstream(path);
}

QList<TrackInfo*> DecoderVgmstreamFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
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

    VgmstreamHelper helper(path);
    if(!helper.initialize())
    {
        qWarning("DecoderVgmstreamFactory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderVgmstreamFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderVgmstreamFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(vgmstream, DecoderVgmstreamFactory)
#endif
