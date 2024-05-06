#include <sidplayfp/SidTune.h>
#include "decodersidfactory.h"
#include "decoder_sid.h"
#include "sidhelper.h"
#include "settingsdialog.h"

#include <QFile>
#include <QSettings>

DecoderSIDFactory::DecoderSIDFactory()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");
    if(settings.value("use_hvsc", false).toBool())
    {
        if(!m_db.open(QmmpPrintable(settings.value("hvsc_path", HVSC_PATH).toString())))
        {
            qWarning("DecoderSIDFactory: %s", m_db.error());
        }
    }
    settings.endGroup();
}

bool DecoderSIDFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    SidTune tune(nullptr);
    tune.load(QmmpPrintable(file->fileName()));
    return tune.getInfo();
}

DecoderProperties DecoderSIDFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("SID Plugin");
    properties.shortName = "sid";
    properties.filters << "*.sid" << "*.psid" << "*.mus" << "*.str" << "*.p00" << "*.prg" << "*.c64";
    properties.description = "SID File";
    properties.protocols << "file" << "sid";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderSIDFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderSID(&m_db, path);
}

QList<TrackInfo*> DecoderSIDFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
{
    SIDHelper helper(&m_db);
    helper.load(path);

    QList<TrackInfo*> playlist = helper.createPlayList(parts);
    if(playlist.isEmpty())
    {
        return playlist;
    }

    if(path.contains("://")) //is it url?
    {
        const int track = path.section("#", -1).toInt();
        if(track > playlist.count() || track < 1)
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
    return playlist;
}

MetaDataModel* DecoderSIDFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderSIDFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(&m_db, parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(sid, DecoderSIDFactory)
#endif
