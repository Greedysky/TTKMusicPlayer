#include "decoder_cue.h"
#include "cuemetadatamodel.h"
#include "cuefile.h"
#include "decodercuefactory.h"

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
    properties.description = tr("Cuesheet Files");
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
        int track = path.section("#", -1).toInt();
        return cueFile.createPlayList(track);
    }
    else
    {
        ignoredPaths->append(cueFile.dataFilePaths());
        return cueFile.createPlayList();
    }
}

MetaDataModel* DecoderCUEFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return path.startsWith("cue://") ? new CUEMetaDataModel(path) : nullptr;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(cue, DecoderCUEFactory)
#endif
