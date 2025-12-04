#include "decodernezplugfactory.h"
#include "decoder_nezplug.h"
#include "nezplughelper.h"
#include "settingsdialog.h"

bool DecoderNEZplugFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    NEZplugHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderNEZplugFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("NEZplug Plugin");
    properties.shortName = "nezplug";
    properties.filters << "*.ay";
    properties.filters << "*.gbr" << "*.gbs";
    properties.filters << "*.hes";
    properties.filters << "*.kss";
    properties.filters << "*.mus";
    properties.filters << "*.sgc";
    properties.filters << "*.nsd" << "*.nsf" << "*.nsfe";
    properties.description = "NEZplug++ Music File";
    properties.protocols << "file" << "nezplug";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderNEZplugFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderNEZplug(path);
}

QList<TrackInfo*> DecoderNEZplugFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
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

    NEZplugHelper helper(path);
    if(!helper.initialize())
    {
        qWarning("DecoderNEZplugFactory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderNEZplugFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderNEZplugFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(nezplug, DecoderNEZplugFactory)
#endif
