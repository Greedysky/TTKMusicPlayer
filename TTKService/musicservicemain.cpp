#include "musicapplication.h"
#include "musicutils.h"
#include "musicxmlconfigmanager.h"
#include "musicnetworkthread.h"

#include <QApplication>
#include <QTranslator>

#define TTK_DEBUG

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifndef TTK_DEBUG
    if(argc <= 1)
    {
        return -1;
    }
    else if(QString(argv[1]) != APPNAME)
    {
        return -1;
    }
#endif
    ///////////////////////////////////////////////////////
#ifndef MUSIC_GREATER_NEW
    MusicUtils::UCore::setLocalCodec();
#endif
#ifdef Q_OS_UNIX
    QFont font;
    font.setPixelSize(13);
    qApp->setFont(font);
#endif

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

    if(argc == 4)
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
