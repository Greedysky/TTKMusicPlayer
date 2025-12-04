#include "decoderopenmptfactory.h"
#include "decoder_openmpt.h"
#include "openmptmetadatamodel.h"
#include "openmpthelper.h"
#include "archivereader.h"
#include "settingsdialog.h"

#include <QSettings>

bool DecoderOpenMPTFactory::canDecode(QIODevice *input) const
{
    OpenMPTHelper helper(input);
    return helper.initialize();
}

DecoderProperties DecoderOpenMPTFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("OpenMPT Plugin");
    properties.shortName = "openmpt";
    properties.filters << "*.669";
    properties.filters << "*.ams" << "*.amf";
    properties.filters << "*.c67";
    properties.filters << "*.dsm" << "*.dmf" << "*.dbm" << "*.digi" << "*.dtm";
    properties.filters << "*.far" << "*.flx" << "*.ft";
    properties.filters << "*.gdm";
    properties.filters << "*.it" << "*.ice" << "*.imf";
    properties.filters << "*.j2b";
    properties.filters << "*.mod" << "*.mptm" << "*.m15" << "*.mtm" << "*.med" << "*.mmd" << "*.mmd0" << "*.mmd1" << "*.mmd2" << "*.mmd3" << "*.mdl" << "*.mt2" << "*.mms" << "*.mo3" << "*.mmcmp";
    properties.filters << "*.nst";
    properties.filters << "*.oct" << "*.okt" << "*.okta";
    properties.filters << "*.pt36" << "*.ptm" << "*.psm" << "*.plm" << "*.ppm";
    properties.filters << "*.s3m" << "*.stm" << "*.stk" << "*.st26" << "*.sfx" << "*.sfx2" << "*.stp";
    properties.filters << "*.ult" << "*.umx" << "*.unic";
    properties.filters << "*.wow";
    properties.filters << "*.xm" << "*.xpk";
    properties.filters << ArchiveReader::filters();
    properties.description = "OpenMPT Module Files";
    properties.hasSettings = true;
    return properties;
}

Decoder *DecoderOpenMPTFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(path);
    return new DecoderOpenMPT(input);
}

QList<TrackInfo*> DecoderOpenMPTFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        delete info;
        return QList<TrackInfo*>();
    }

    OpenMPTHelper helper(&file);
    if(!helper.initialize())
    {
        file.close();
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        if(settings.value("OpenMPT/use_file_name", false).toBool())
        {
            info->setValue(Qmmp::TITLE, path.section('/', -1));
        }
        else
        {
            info->setValue(Qmmp::TITLE, helper.title());
        }
        info->setValue(Qmmp::COMMENT, helper.comment());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "OpenMPT");
        info->setDuration(helper.totalTime());
    }

    file.close();
    return QList<TrackInfo*>() << info;
}

MetaDataModel *DecoderOpenMPTFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new OpenMPTMetaDataModel(path);
}

QDialog *DecoderOpenMPTFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(openmpt, DecoderOpenMPTFactory)
#endif
