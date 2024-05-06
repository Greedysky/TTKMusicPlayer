#include "decoderarchivefactory.h"
#include "decoder_archive.h"
#include "archivereader.h"
#include "settingsdialog.h"

bool DecoderArchiveFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderArchiveFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Archive Plugin");
    properties.shortName = "archive";
    properties.filters << A7zipReader::filters();
    properties.filters << RSNReader::filters();
    properties.description = "Archive File";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderArchiveFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderArchive(path);
}

QList<TrackInfo*> DecoderArchiveFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    QList<TrackInfo*> playlist;
    if(!ArchiveReader::unpack(path))
    {
        return playlist;
    }

    for(const QString &file : ArchiveReader::list(path))
    {
        //is this file supported by qmmp?
        for(DecoderFactory *factory : Decoder::findByFileExtension(file))
        {
            if(factory->properties().shortName == "archive")
            {
                continue;
            }

            QList<TrackInfo*> list = factory->createPlayList(file, parts, nullptr);
            if(!list.isEmpty())
            {
                playlist << list;
                break;
            }
        }
    }
    return playlist;
}

MetaDataModel* DecoderArchiveFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderArchiveFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(archive, DecoderArchiveFactory)
#endif
