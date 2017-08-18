#include "musicbackgroundimage.h"
#include "musicnumberdefine.h"
#include "musicotherdefine.h"
#include "musicwidgetutils.h"

#include "ttkzip/zip.h"
#include "ttkzip/unzip.h"

#include <QFile>

#define WIN_NAME_MAX_LENGTH     256
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

void MusicSkinConfigManager::writeSkinXMLConfig(const MusicSkinConfigItem &item, const QString &path)
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

void MusicSkinConfigManager::readSkinXMLConfig(MusicSkinConfigItem &item)
{
    item.m_name = readXmlAttributeByTagNameValue("name");
    item.m_useCount = readXmlAttributeByTagNameValue("useCount").toInt();
}



QString MusicBackgroundImageWrap::getClassName()
{
    return "MusicBackgroundImageWrap";
}

bool MusicBackgroundImageWrap::outputSkin(QPixmap &image, const QString &path)
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
        char file[WIN_NAME_MAX_LENGTH] = {0};
        char ext[WIN_NAME_MAX_LENGTH] = {0};
        char com[MH_KB] = {0};

        if(unzGetCurrentFileInfo64(zFile, &fileInfo, file, sizeof(file), ext, WIN_NAME_MAX_LENGTH, com, MH_KB) != UNZ_OK)
        {
            break;
        }

        if(unzOpenCurrentFile(zFile) != UNZ_OK)
        {
            break;
        }

        char data[MH_KB] = {0};
        int size = 0;

        QByteArray arrayData;
        if(QString(file).toLower().contains("image/bkg"))
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
            image.loadFromData(arrayData);
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

bool MusicBackgroundImageWrap::outputSkin(MusicBackgroundImage &image, const QString &path)
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
        char file[WIN_NAME_MAX_LENGTH] = {0};
        char ext[WIN_NAME_MAX_LENGTH] = {0};
        char com[MH_KB] = {0};

        if(unzGetCurrentFileInfo64(zFile, &fileInfo, file, sizeof(file), ext, WIN_NAME_MAX_LENGTH, com, MH_KB) != UNZ_OK)
        {
            break;
        }

        if(unzOpenCurrentFile(zFile) != UNZ_OK)
        {
            break;
        }

        char data[MH_KB] = {0};
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
            while(true)
            {
                size= unzReadCurrentFile(zFile, data, sizeof(data));
                if(size <= 0)
                {
                    break;
                }
                arrayData.append(data, size);
            }

            MusicSkinConfigManager manager;
            MusicSkinConfigItem item;
            if(manager.fromByteArray(arrayData))
            {
                manager.readSkinXMLConfig(item);
                image.m_item = item;
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

bool MusicBackgroundImageWrap::inputSkin(const MusicBackgroundImage &image, const QString &path)
{
    zipFile zFile = zipOpen64(path.toLocal8Bit().constData(), 0);
    if(NULL == zFile)
    {
        return false;
    }

    QString nPrefix = QFileInfo(path).baseName();
    int level = 5;

    zip_fileinfo fileInfo;
    memset(&fileInfo, 0, sizeof(fileInfo));

    zipOpenNewFileInZip(zFile, (nPrefix + SKN_FILE).toLocal8Bit().constData(), &fileInfo, NULL, 0, NULL, 0, NULL, Z_DEFLATED, level);
    QByteArray data = MusicUtils::Widget::getPixmapData(image.m_pix);
    zipWriteInFileInZip(zFile, data.constData(), data.size());
    zipCloseFileInZip(zFile);

    MusicSkinConfigManager manager;
    manager.writeSkinXMLConfig(image.m_item, MUSIC_IMAGE_FILE);
    data = manager.toByteArray();

    zipOpenNewFileInZip(zFile, (nPrefix + XML_FILE).toLocal8Bit().constData(), &fileInfo, NULL, 0, NULL, 0, NULL, Z_DEFLATED, level);
    zipWriteInFileInZip(zFile, data.constData(), data.size());
    zipCloseFileInZip(zFile);
    QFile::remove(MUSIC_IMAGE_FILE);

    zipClose(zFile, 0);

    return true;
}
