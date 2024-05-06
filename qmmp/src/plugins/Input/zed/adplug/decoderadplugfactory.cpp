#include "decoderadplugfactory.h"
#include "adplughelper.h"
#include "decoder_adplug.h"
#include "adplugmetadatamodel.h"
#include "settingsdialog.h"

bool DecoderAdPlugFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    AdPlugHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderAdPlugFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("AdPlug Plugin");
    properties.shortName = "adplug";
    properties.filters << "*.a2m" << "*.adl" << "*.adlib" << "*.agd" << "*.amd" << "*.as3m";
    properties.filters << "*.bam" << "*.bmf";
    properties.filters << "*.cff" << "*.cmf";
    properties.filters << "*.d00" << "*.dfm" << "*.dmo" << "*.dro" << "*.dtm";
    properties.filters << "*.got";
    properties.filters << "*.ha2" << "*.hsc" << "*.hsq" << "*.hsp";
    properties.filters << "*.imf" << "*.ims";
    properties.filters << "*.jbm";
    properties.filters << "*.laa" << "*.lds";
    properties.filters << "*.m" << "*.mad" << "*.mdi" << "*.mkj" << "*.msc" << "*.mtk" << "*.mus";
    properties.filters << "*.rac" << "*.rad" << "*.raw" << "*.rix";
    properties.filters << "*.s3m" << "*.sa2" << "*.sat" << "*.sdb" << "*.sng" << "*.sop" << "*.sqx";
    properties.filters << "*.xad" << "*.xms" << "*.xsm";
    properties.filters << "*.vgm" << "*.vgz";
    properties.filters << "*.wlf";
    // pair suffix section
    properties.filters << "*.ksm"; // (ksm, dat)
    properties.filters << "*.rol"; // (rol, bnk)
    properties.filters << "*.sci"; // (sci, name)
    properties.description = "AdLib Sound File";
    properties.protocols << "file";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderAdPlugFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderAdPlug(path);
}

QList<TrackInfo*> DecoderAdPlugFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    AdPlugHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::ARTIST, helper.author());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "AdLib Sound");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel *DecoderAdPlugFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new AdPlugMetaDataModel(path);
}

QDialog *DecoderAdPlugFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(adplug, DecoderAdPlugFactory)
#endif
