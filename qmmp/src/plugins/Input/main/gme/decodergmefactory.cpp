#include "decodergmefactory.h"
#include "gmehelper.h"
#include "decoder_gme.h"
#include "settingsdialog.h"

bool DecoderGMEFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    GMEHelper helper;
    return helper.load(file->fileName());
}

DecoderProperties DecoderGMEFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("GME Plugin");
    properties.shortName = "gme";
    properties.filters << "*.ay" << "*.gbs" << "*.gym" << "*.hes" << "*.kss" << "*.nsf" << "*.nsfe";
    properties.filters << "*.sap" << "*.spc" << "*.vgm" << "*.vgz";
    properties.description = "Game Music Emulator File";
    properties.protocols << "file" << "gme";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderGMEFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderGME(path);
}

QList<TrackInfo*> DecoderGMEFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
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

    GMEHelper helper;
    Music_Emu *emu = helper.load(path);
    if(!emu)
    {
        qWarning("DecoderGMEFactory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderGMEFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderGMEFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(gme, DecoderGMEFactory)
#endif
