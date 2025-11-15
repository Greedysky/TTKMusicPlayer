#include "decoderuadefactory.h"
#include "uademetadatamodel.h"
#include "decoder_uade.h"
#include "uadehelper.h"

bool DecoderUADEFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    UADEHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderUADEFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("UADE Plugin");
    properties.shortName = "uade";
    properties.filters << UADEHelper::filters();
    properties.description = "Unix Amiga Delitracker Emulator File";
    properties.protocols << "file" << "uade";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderUADEFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderUADE(path);
}

QList<TrackInfo*> DecoderUADEFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
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

    UADEHelper helper(path);
    if(!helper.initialize())
    {
        qWarning("DecoderUADEFactory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderUADEFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new UADEMetaDataModel(path);
}

QDialog *DecoderUADEFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(uade, DecoderUADEFactory)
#endif
