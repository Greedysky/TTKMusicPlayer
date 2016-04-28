#include "musicapplication.h"
#include "musicutils.h"
#include "musicxmlconfigmanager.h"
#include "musicnetworkthread.h"
#include "musiclogger.h"
#include "musicmessagebox.h"

#include <QApplication>
#include <QTranslator>
//#include <vld.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifndef MUSIC_QT_5
    MusicUtils::setLocalCodec();
#endif
#ifdef Q_OS_UNIX
    QFont font;
    font.setPixelSize(13);
    qApp->setFont(font);
#endif

    MusicUtils::checkTheDirectoryExist();
    if(!MusicUtils::checkTheFileExist())
    {
        M_LOGGER_INFO("Load Translation");
        QTranslator translator;
        translator.load(MusicUtils::getLanguageName(0));
        a.installTranslator(&translator);

        MusicMessageBox(QObject::tr("TTKMusicPlayer"),
                        QObject::tr("Lack of necessary component files!")
                        ).exec();
        return -1;
    }//check file error!

    ///////////////////////////////////////////////////////
    M_LOGGER_INFO("MusicApplication Begin");
    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    //detect the current network state
    M_NETWORK->start();

    M_LOGGER_INFO("Load Translation");
    MusicXMLConfigManager *xml = new MusicXMLConfigManager;
    xml->readXMLConfig();
    QTranslator translator;
    translator.load(MusicUtils::getLanguageName(xml->readLanguageIndex()));
    a.installTranslator(&translator);

    MusicUtils::checkCacheSize(xml->readDownloadCacheSize()*1024*1024,
                              xml->readDownloadCacheLimit(), MUSIC_DOWNLOAD_AL);
    M_NETWORK->setBlockNetWork(xml->readCloseNetworkConfig());
    delete xml;
    M_LOGGER_INFO("End load translation");

    MusicApplication w;
    w.show();

    if(argc == 3)
    {
        if( QString(argv[1]) == "-Open" )
        {
            w.musicImportSongsSettingPath(QStringList() << argv[2]);
            QTimer::singleShot(1, &w, SLOT(musicImportPlay()));
        }
        if( QString(argv[1]) == "-List" )
        {
            w.musicImportSongsSettingPath(QStringList() << argv[2]);
        }
    }

    return a.exec();
}
