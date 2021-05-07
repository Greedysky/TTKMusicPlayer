#include "decoderadplugfactory.h"
#include "adplughelper.h"
#include "decoder_adplug.h"
#include "magic.h"
#include "adplugmetadatamodel.h"

class InputStreamQIO : public InputStream
{
public:
    explicit InputStreamQIO(QIODevice *i)
        : m_device(i)
    {

    }

    virtual bool read(std::size_t offset, void *buf, std::size_t n) override
    {
        qint64 ret;
        if(!m_device->seek(offset))
        {
            return false;
        }

        ret = m_device->read(reinterpret_cast<char *>(buf), n);
        return ret > 0 && ret == (qint64)n;
    }

    virtual size_t size() override
    {
        if(m_device->isSequential() || !m_device->isOpen())
        {
            throw InputStream::NoRandomAccess();
        }
        return m_device->size();
    }

private:
    QIODevice *m_device;

};


bool DecoderAdplugFactory::canDecode(QIODevice *input) const
{
    return adplug_supports(InputStreamQIO(input));
}

DecoderProperties DecoderAdplugFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("AdPlug Plugin");
    properties.filters << "*.adl" << "*.hsc" << "*.ksm" << "*.lds" << "*.amd" << "*.d00" << "*.rad";
    properties.description = tr("AdLib Sound Files");
    properties.shortName = "adplug";
    return properties;
}

Decoder *DecoderAdplugFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderAdplug(path);
}

QList<TrackInfo*> DecoderAdplugFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    AdplugHelper helper(qUtf8Printable(path));
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, QString::fromStdString(helper.title()));
        info->setValue(Qmmp::ARTIST, QString::fromStdString(helper.author()));
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.rate());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::FORMAT_NAME, QString::fromStdString(helper.format()));
        info->setDuration(helper.length());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel *DecoderAdplugFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new AdplugMetaDataModel(path);
}

void DecoderAdplugFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(adplug, DecoderAdplugFactory)
#endif
