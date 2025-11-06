#include "decoderorganyafactory.h"
#include "organyahelper.h"
#include "decoder_organya.h"

bool DecoderOrganyaFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    OrganyaHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderOrganyaFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Organya Plugin");
    properties.shortName = "organya";
    properties.filters << "*.org" << "*.pttune" << "*.ptcop";
    properties.description = "Cave Story's org Audio File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderOrganyaFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderOrganya(path);
}

QList<TrackInfo*> DecoderOrganyaFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    OrganyaHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "Cave Story's org");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderOrganyaFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderOrganyaFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(organya, DecoderOrganyaFactory)
#endif
