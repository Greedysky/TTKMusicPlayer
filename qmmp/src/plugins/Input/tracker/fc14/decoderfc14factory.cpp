#include "decoderfc14factory.h"
#include "decoder_fc14.h"
#include "fc14helper.h"

bool DecoderFC14Factory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    FC14Helper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderFC14Factory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("FC14 Plugin");
    properties.shortName = "fc14";
    properties.filters << "*.fc" << "*.fc13" << "*.fc14" << "*.smod";
#ifdef LIBFC_VERSION2
    properties.filters << "*.hip" << "*.hip7" << "*.hipc" << "*.mcmd";
#endif
    properties.description = "Future Composer Audio File";
    properties.protocols << "file" << "fc14";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderFC14Factory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderFC14(path);
}

QList<TrackInfo*> DecoderFC14Factory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
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

    FC14Helper helper(path);
    if(!helper.initialize())
    {
        qWarning("DecoderFC14Factory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderFC14Factory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderFC14Factory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(fc14, DecoderFC14Factory)
#endif
