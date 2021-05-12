#include <QSettings>
#include <cdio/version.h>
#include <cddb/version.h>
#include "decoder_cdaudio.h"
#include "decodercdaudiofactory.h"

DecoderCDAudioFactory::DecoderCDAudioFactory()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    if(settings.value("cdaudio/cddb_server").toByteArray() == "freedb.org")
    {
        settings.setValue("cdaudio/cddb_server", "gnudb.org");
    }
}

bool DecoderCDAudioFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderCDAudioFactory::properties() const
{
    DecoderProperties properties;
    properties.name = "CD Audio Plugin";
    properties.shortName = "cdaudio";
    properties.description = "Compact Disc Input and Control File";
    properties.protocols << "cdda";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderCDAudioFactory::create(const QString &url, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderCDAudio(url);
}

QList<TrackInfo*> DecoderCDAudioFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    QList<TrackInfo*> list;

    if(path.contains("#"))
        return list;

    QString device_path = path;
    device_path.remove("cdda://");
    const QList<CDATrack> tracks = DecoderCDAudio::generateTrackList(device_path, parts);
    for(const CDATrack &t : qAsConst(tracks))
    {
        list << new TrackInfo(t.info);
    }
    return list;
}

MetaDataModel* DecoderCDAudioFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    Q_UNUSED(path);
    return nullptr;
}

void DecoderCDAudioFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(cdaudio, DecoderCDAudioFactory)
#endif
