#include "decoderkssfactory.h"
#include "decoder_kss.h"
#include "ksshelper.h"
#include "settingsdialog.h"

bool DecoderKSSFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    KSSHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderKSSFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("KSS Plugin");
    properties.shortName = "kss";
    properties.filters << "*.kss" << "*.mgs" << "*.bgm" << "*.opx" << "*.mpk" << "*.mbm";
    properties.description = "MSX Music File";
    properties.protocols << "file" << "kss";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderKSSFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderKSS(path);
}

QList<TrackInfo*> DecoderKSSFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
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

    KSSHelper helper(path);
    if(!helper.initialize())
    {
        qWarning("DecoderKSSFactory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderKSSFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderKSSFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(kss, DecoderKSSFactory)
#endif
