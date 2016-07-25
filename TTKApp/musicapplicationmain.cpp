#include "musicapplication.h"
#include "musicutils.h"
#include "musicxmlconfigmanager.h"
#include "musicnetworkthread.h"
#include "musicmessagebox.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifndef MUSIC_GREATER_NEW
    MusicUtils::UCore::setLocalCodec();
#endif
#ifdef Q_OS_UNIX
    QFont font;
    font.setPixelSize(13);
    qApp->setFont(font);
#endif

    MusicUtils::UCore::checkTheDirectoryExist();
    if(!MusicUtils::UCore::checkTheFileExist())
    {
        M_LOGGER_INFO("Load Translation");
        QTranslator translator;
        translator.load(MusicUtils::UCore::getLanguageName(0));
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
    M_NETWORK_PTR->start();

    M_LOGGER_INFO("Load Translation");
    MusicXMLConfigManager *xml = new MusicXMLConfigManager;
    xml->readXMLConfig();
    QTranslator translator;
    translator.load(MusicUtils::UCore::getLanguageName(xml->readLanguageIndex()));
    a.installTranslator(&translator);

    MusicUtils::UCore::checkCacheSize(xml->readDownloadCacheSize()*MH_MB2B,
                                      xml->readDownloadCacheLimit(), MUSIC_DIR_FULL);
    M_NETWORK_PTR->setBlockNetWork(xml->readCloseNetworkConfig());
    delete xml;
    M_LOGGER_INFO("End load translation");

    MusicApplication w;
    w.show();

    if(argc == 3)
    {
        if( QString(argv[1]) == "-Open" )
        {
            w.musicImportSongsSettingPath(QStringList() << argv[2]);
            QTimer::singleShot(MT_MS, &w, SLOT(musicImportPlay()));
        }
        if( QString(argv[1]) == "-List" )
        {
            w.musicImportSongsSettingPath(QStringList() << argv[2]);
        }
    }

    return a.exec();
}
