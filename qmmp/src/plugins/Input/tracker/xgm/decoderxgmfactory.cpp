#include "decoderxgmfactory.h"
#include "xgmhelper.h"
#include "decoder_xgm.h"
#include "settingsdialog.h"

bool DecoderXGMFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    XGMHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderXGMFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("XGM Plugin");
    properties.shortName = "xgm";
    // kss
    properties.filters << "*.kss" << "*.mgs" << "*.bgm" << "*.opx" << "*.mpk" << "*.mbm";
    // nezplug++
    properties.filters << "*.ay" << "*.gbr" << "*.gbs" << "*.hes" << "*.mus" << "*.nsd" << "*.nsf" << "*.nsfe" << "*.sgc";
    // jaytrax
    properties.filters << "*.jxs";
    // pac
    properties.filters << "*.pac";
    properties.description = "MSX Related Music File";
    properties.protocols << "file" << "xgm";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderXGMFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderXGM(path);
}

QList<TrackInfo*> DecoderXGMFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
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

    XGMHelper helper(path);
    if(!helper.initialize())
    {
        qWarning("DecoderXGMFactory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderXGMFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderXGMFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(xgm, DecoderXGMFactory)
#endif
