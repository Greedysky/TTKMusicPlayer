#include "decodercdaudiofactory.h"
#include "decoder_cdaudio.h"
#include "settingsdialog.h"

#include <QSettings>

DecoderCDAudioFactory::DecoderCDAudioFactory()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    if(settings.value("CDAudio/cddb_server").toByteArray() == "freedb.org")
    {
        settings.setValue("CDAudio/cddb_server", "gnudb.org");
    }
}

bool DecoderCDAudioFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderCDAudioFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("CD Audio Plugin");
    properties.shortName = "cdaudio";
    properties.description = "Compact Disc Input and Control File";
    properties.protocols << "cdda";
    properties.noInput = true;
    properties.hasSettings = true;
    return properties;
}

Decoder *DecoderCDAudioFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderCDAudio(path);
}

QList<TrackInfo*> DecoderCDAudioFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    QList<TrackInfo*> playlist;
    if(path.contains("#"))
    {
        return playlist;
    }

    QString device_path = path;
    device_path.remove("cdda://");

    const QList<CDATrack> tracks = DecoderCDAudio::generateTrackList(device_path, parts);
    for(const CDATrack &t : qAsConst(tracks))
    {
        playlist << new TrackInfo(t.info);
    }
    return playlist;
}

MetaDataModel* DecoderCDAudioFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    Q_UNUSED(path);
    return nullptr;
}

QDialog *DecoderCDAudioFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(cdaudio, DecoderCDAudioFactory)
#endif
