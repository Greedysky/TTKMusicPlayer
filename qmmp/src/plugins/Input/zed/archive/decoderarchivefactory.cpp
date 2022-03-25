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
    properties.name = tr("7z Archive Plugin");
    properties.shortName = "archive";
    properties.filters << "*.7z" << "*.xz" << "*.bzip2" << "*.gzip" << "*.tar" << "*.zip" << "*.wim";
    properties.filters << "*.ar" << "*.arj";
    properties.filters << "*.cab" << "*.chm" << "*.cpio" << "*.cramfs";
    properties.filters << "*.dmg";
    properties.filters << "*.ext";
    properties.filters << "*.fat";
    properties.filters << "*.gpt";
    properties.filters << "*.hfs";
    properties.filters << "*.ihex" << "*.iso";
    properties.filters << "*.lzh" << "*.lzma";
    properties.filters << "*.mbr" << "*.msi";
    properties.filters << "*.nsis" << "*.ntfs";
    properties.filters << "*.qcow2";
    properties.filters << "*.rar" << "*.rpm";
    properties.filters << "*.squashfs";
    properties.filters << "*.udf" << "*.uefi";
    properties.filters << "*.vdi" << "*.vhd" << "*.vmdk";
    properties.filters << "*.xar";
    properties.filters << "*.z";
    properties.description = "7z Archive File";
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

void DecoderArchiveFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(archive, DecoderArchiveFactory)
#endif
