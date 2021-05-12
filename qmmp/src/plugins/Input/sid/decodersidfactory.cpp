#include <QSettings>
#include <sidplayfp/SidTune.h>
#include <sidplayfp/SidTuneInfo.h>
#include "decoder_sid.h"
#include "sidhelper.h"
#include "decodersidfactory.h"
#include "settingsdialog.h"

DecoderSIDFactory::DecoderSIDFactory()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");
    if(settings.value("use_hvsc", false).toBool())
    {
        QString default_path = Qmmp::configDir() + "/Songlengths.txt";
        if(!m_db.open(qPrintable(settings.value("hvsc_path", default_path).toString())))
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
    properties.name = "SID Plugin";
    properties.shortName = "sid";
    properties.filters << "*.sid" << "*.mus" << "*.str" << "*.prg" << "*.P00" << "*.c64";
    properties.description = "SID File";
    properties.hasSettings = true;
    properties.noInput = true;
    properties.protocols << "sid";
    return properties;
}

Decoder *DecoderSIDFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderSID(&m_db, path);
}

QList<TrackInfo*> DecoderSIDFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredFiles)
{
    SIDHelper helper(&m_db);
    helper.load(path);

    QList<TrackInfo*> list = helper.createPlayList(parts);
    if(list.isEmpty())
    {
        return list;
    }

    if(path.contains("://")) //is it url?
    {
        const int track = path.section("#", -1).toInt();
        if(track > list.count() || track < 1)
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

    return list;
}

MetaDataModel* DecoderSIDFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderSIDFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(&m_db, parent);
    s->show();
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(sid, DecoderSIDFactory)
#endif
