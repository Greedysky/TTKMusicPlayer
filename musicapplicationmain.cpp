#include "musicapplication.h"
#include "musicobject.h"
#include "musicxmlconfigmanager.h"
#include "musicnetworkthread.h"
#include "musiclogger.h"

#include <QApplication>
#include <QTranslator>
//#include <vld.h>


quint64 dirSize(const QString& dirName)
{
    quint64 size = 0;
    if(QFileInfo(dirName).isDir())
    {
        QDir dir(dirName);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs |  QDir::Hidden |
                                               QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for(int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            if(fileInfo.isDir())
            {
                size += dirSize(fileInfo.absoluteFilePath());
            }
            else
                size += fileInfo.size();
        }
    }
    return size;
}

void checkCacheSize(quint64 cacheSize, bool disabled, const QString &path)
{
    if(disabled)
    {
        quint64 size = dirSize( path );
        if( size > cacheSize)
        {
            QFileInfoList fileList = QDir(path).entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
            for(int i=0; i<fileList.count(); ++i)
            {
                size -= fileList[i].size();
                QFile::remove(fileList[i].absoluteFilePath());
                if(size <= cacheSize)
                {
                    break;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    M_LOOGERS("MusicApplication Begin");
    QCoreApplication::setOrganizationName("QMusicPlayer");
    QCoreApplication::setOrganizationDomain("QMusicPlayer.com");
    QCoreApplication::setApplicationName("QMusicPlayer");

    //detect the current network state
    M_NETWORK->start();

    M_LOOGERS("Load Translation");
    MusicXMLConfigManager *xml = new MusicXMLConfigManager;
    xml->readXMLConfig();
    QTranslator translator;
    translator.load(MusicObject::getLanguageName(xml->readLanguageIndex()));
    a.installTranslator(&translator);

    checkCacheSize(xml->readDownloadCacheSize()*1024*1024,
                   xml->readDownloadCacheLimit(),
                   MusicObject::getAppDir() + MUSIC_DOWNLOAD);

    M_NETWORK->setBlockNetWork(xml->readCloseNetworkConfig());
    delete xml;
    M_LOOGERS("End load translation");

    MusicObject::checkTheDirectoryExist();
    MusicApplication w;
    w.show();

    if(argc == 3)
    {
        if( QString(argv[1]) == "-Open" )
        {
            w.musicImportSongsSettingPath(QStringList() << argv[2]);
            w.musicImportPlay();
        }
        if( QString(argv[1]) == "-List" )
        {
            w.musicImportSongsSettingPath(QStringList() << argv[2]);
        }
    }

    return a.exec();
}
