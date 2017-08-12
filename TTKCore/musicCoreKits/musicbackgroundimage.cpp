#include "musicbackgroundimage.h"

#include "ttkzip/zip.h"
#include "ttkzip/unzip.h"

#include <QFile>
#include <QBuffer>

#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wsign-compare"
#endif

MusicSkinConfigManager::MusicSkinConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicSkinConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicSkinConfigManager::writeMusicSongsConfig(const MusicSkinConfigItem &item, const QString &path)
{
    //Open wirte file
    if( !writeConfig( path ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    ///////////////////////////////////////////////////////
    QDomElement musicPlayerDom = createRoot("TTKSkin");
    //Class A
    writeDomElement(musicPlayerDom, "creator", MusicXmlAttribute("value", APPNAME));
    writeDomElement(musicPlayerDom, "name", MusicXmlAttribute("value", item.m_name));
    writeDomElement(musicPlayerDom, "useCount", MusicXmlAttribute("value", item.m_useCount));

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicSkinConfigManager::readMusicSongsConfig(MusicSkinConfigItem &item)
{
    item.m_name = readXmlAttributeByTagNameValue("name");
    item.m_useCount = readXmlAttributeByTagNameValue("useCount").toInt();
}



QString MusicBackgroundImageCore::getClassName()
{
    return "MusicBackgroundImageCore";
}

bool MusicBackgroundImageCore::outputSkin(MusicBackgroundImage &image, const QString &path)
{
    unzFile zFile = unzOpen64(path.toLocal8Bit().constData());
    if(NULL == zFile)
    {
        return false;
    }

    unz_file_info64 fileInfo;
    unz_global_info64 gInfo;
    if (unzGetGlobalInfo64(zFile, &gInfo) != UNZ_OK)
    {
        return false;
    }

    for(int i=0; i<gInfo.number_entry; ++i)
    {
        char file[256] = {0};
        char ext[256] = {0};
        char com[1024] = {0};

        if(unzGetCurrentFileInfo64(zFile, &fileInfo, file, sizeof(file), ext, 256, com, 1024) != UNZ_OK)
        {
            break;
        }

        if(unzOpenCurrentFile(zFile) != UNZ_OK)
        {
            break;
        }

        char data[1024] = {0};
        int size = 0;

        QByteArray arrayData;
        if(QString(file).toLower().contains(SKN_FILE))
        {
            while(true)
            {
                size= unzReadCurrentFile(zFile, data, sizeof(data));
                if(size <= 0)
                {
                    break;
                }
                arrayData.append(data, size);
            }

            QPixmap pix;
            pix.loadFromData(arrayData);
            image.m_pix = pix;
        }
        else if(QString(file).toLower().contains(XML_FILE))
        {
            MusicSkinConfigManager manager;
            MusicSkinConfigItem item;
            if(manager.readConfig(file))
            {
                manager.readMusicSongsConfig(item);
                image.m_name = item.m_name;
                image.m_useCount = item.m_useCount;
            }
        }

        unzCloseCurrentFile(zFile);

        if(i < gInfo.number_entry - 1 && unzGoToNextFile(zFile) != UNZ_OK)
        {
            return false;
        }
    }
    unzClose(zFile);

    return true;
}

bool MusicBackgroundImageCore::inputSkin(const MusicBackgroundImage &image, const QString &path)
{
    zipFile zFile = zipOpen64(path.toLocal8Bit().constData(), 0);
    if(NULL == zFile)
    {
        return false;
    }

    QString nPrefix(path);
    nPrefix.chop(5);
    int level = 5;

    zip_fileinfo fileInfo;
    memset(&fileInfo, 0, sizeof(fileInfo));

    zipOpenNewFileInZip(zFile, (nPrefix + SKN_FILE).toLocal8Bit().constData(), &fileInfo, NULL, 0, NULL, 0, NULL, Z_DEFLATED, level);
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    image.m_pix.save(&buffer, JPG_FILE_PREFIX);
    zipWriteInFileInZip(zFile, data.constData(), data.size());
    zipCloseFileInZip(zFile);

    MusicSkinConfigItem sss;
    sss.m_name = image.m_name;
    sss.m_useCount = image.m_useCount;
    MusicSkinConfigManager manager;
    manager.writeMusicSongsConfig(sss, TEMPORARY_DIR);
    data = manager.toByteArray();

    zipOpenNewFileInZip(zFile, (nPrefix + XML_FILE).toLocal8Bit().constData(), &fileInfo, NULL, 0, NULL, 0, NULL, Z_DEFLATED, level);
    zipWriteInFileInZip(zFile, data.constData(), data.size());
    zipCloseFileInZip(zFile);
    QFile::remove(TEMPORARY_DIR);

    zipClose(zFile, 0);

    return true;
}
