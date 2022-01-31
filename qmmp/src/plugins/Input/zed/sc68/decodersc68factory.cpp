#include "decodersc68factory.h"
#include "sc68helper.h"
#include "decoder_sc68.h"

bool DecoderSC68Factory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderSC68Factory::properties() const
{
    DecoderProperties properties;
    properties.name = "SC68 Plugin";
    properties.shortName = "sc68";
    properties.filters << "*.sc68";
    properties.description = "Atari ST(E) And Amiga Audio File";
    properties.protocols << "sc68";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderSC68Factory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderSC68(path);
}

QList<TrackInfo*> DecoderSC68Factory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredFiles)
{
    if(path.contains("://")) //is it one track?
    {
        QString filePath = path;
        filePath.remove("sc68://");
        filePath.remove(RegularWrapper("#\\d+$"));

        const int track = path.section("#", -1).toInt();
        QList<TrackInfo*> list = createPlayList(filePath, parts, ignoredFiles);
        if(list.isEmpty() || track <= 0 || track > list.count())
        {
            qDeleteAll(list);
            list.clear();
            return list;
        }

        TrackInfo *info = list.takeAt(track - 1);
        qDeleteAll(list);
        return QList<TrackInfo*>() << info;
    }
    else
    {
        if(ignoredFiles)
            ignoredFiles->push_back(path);
    }

    SC68Helper helper(path);
    if(!helper.initialize())
    {
        qWarning("DecoderSC68Factory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderSC68Factory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderSC68Factory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(sc68, DecoderSC68Factory)
#endif
