#include "decoders98factory.h"
#include "s98metadatamodel.h"
#include "s98helper.h"
#include "decoder_s98.h"

bool DecoderS98Factory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    S98Helper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderS98Factory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("S98 Plugin");
    properties.shortName = "s98";
    properties.filters << "*.s98" << "*.cym" << "*.mym" << "*.x1f";
    properties.description = "S98 Audio File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderS98Factory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderS98(path);
}

QList<TrackInfo*> DecoderS98Factory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    S98Helper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if((parts & TrackInfo::MetaData) && helper.hasTags())
    {
        info->setValue(Qmmp::TITLE, helper.tag("title"));
        info->setValue(Qmmp::ARTIST, helper.tag("artist"));
        info->setValue(Qmmp::COMMENT, helper.tag("comment"));
        info->setValue(Qmmp::GENRE, helper.tag("genre"));
        info->setValue(Qmmp::COMPOSER, helper.tag("s98by"));
        info->setValue(Qmmp::YEAR, helper.tag("year"));
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "S98");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderS98Factory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new S98MetaDataModel(path);
}

QDialog *DecoderS98Factory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(s98, DecoderS98Factory)
#endif
