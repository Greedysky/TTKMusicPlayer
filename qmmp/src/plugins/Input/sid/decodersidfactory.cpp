#include <sidplayfp/SidTune.h>
#include "decodersidfactory.h"
#include "decoder_sid.h"
#include "sidhelper.h"
#include "settingsdialog.h"

#include <QSettings>

DecoderSIDFactory::DecoderSIDFactory()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");
    if(settings.value("use_hvsc", false).toBool())
    {
        QString default_path = Qmmp::configDir() + "/Songlengths.txt";
        if(!m_db.open(QmmpPrintable(settings.value("hvsc_path", default_path).toString())))
            qWarning("DecoderSIDFactory: %s", m_db.error());
    }
    settings.endGroup();
}

bool DecoderSIDFactory::canDecode(QIODevice *input) const
{
    char buf[4];
    if(input->peek(buf, 4) != 4)
        return false;
    return (!memcmp(buf, "RSID", 4) || !memcmp(buf, "PSID", 4));
}

DecoderProperties DecoderSIDFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("SID Plugin");
    properties.shortName = "sid";
    properties.filters << "*.sid" << "*.mus" << "*.str" << "*.prg" << "*.P00" << "*.c64";
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

void DecoderSIDFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(&m_db, parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(sid, DecoderSIDFactory)
#endif
