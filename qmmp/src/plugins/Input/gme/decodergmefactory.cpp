#include "decodergmefactory.h"
#include "gmehelper.h"
#include "decoder_gme.h"
#include "settingsdialog.h"

bool DecoderGMEFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderGMEFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("GME Plugin");
    properties.shortName = "gme";
    properties.filters << "*.ay" << "*.gbs" << "*.gym" << "*.hes" << "*.kss" << "*.nsf" << "*.nsfe";
    properties.filters << "*.sap" << "*.spc" << "*.vgm" << "*.vgz";
    properties.description = "Game Music Emulator File";
    properties.protocols << "gme";
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
        QString filePath = path;
        filePath.remove("gme://");
        filePath.remove(RegularWrapper("#\\d+$"));

        const int track = path.section("#", -1).toInt();
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

void DecoderGMEFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(gme, DecoderGMEFactory)
#endif
