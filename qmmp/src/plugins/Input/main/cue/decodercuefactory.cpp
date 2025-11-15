#include "decodercuefactory.h"
#include "decoder_cue.h"
#include "cuemetadatamodel.h"
#include "cuefile.h"
#include "settingsdialog.h"

bool DecoderCUEFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderCUEFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("CUE Plugin");
    properties.shortName = "cue";
    properties.filters << "*.cue";
    properties.description = "Cuesheet File";
    properties.protocols << "cue";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderCUEFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderCUE(path);
}

QList<TrackInfo*> DecoderCUEFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
{
    Q_UNUSED(parts);
    CueFile cueFile(path);
    if(path.contains("://"))
    {
        const int track = path.section("#", -1).toInt();
        return cueFile.createPlayList(track);
    }

    if(ignoredPaths)
    {
        ignoredPaths->append(cueFile.dataFilePaths());
    }
    return cueFile.createPlayList();
}

MetaDataModel* DecoderCUEFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    return path.startsWith("cue://") ? new CUEMetaDataModel(readOnly, path) : nullptr;
}

QDialog *DecoderCUEFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(cue, DecoderCUEFactory)
#endif
