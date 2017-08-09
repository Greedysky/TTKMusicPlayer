#include "musicapplication.h"
#include "musiccoreutils.h"
#include "musicxmlconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicnetworkthread.h"
#include "musicversion.h"

#include <QTranslator>
#include <QApplication>

#define TTK_DEBUG

#ifndef Q_OS_UNIX
#include "minidumper.h"
MiniDumper dumper(L"TTK", TTKMUSIC_VERSION_STRW);
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if !defined TTK_DEBUG && !defined Q_OS_UNIX
    if(argc <= 1 || QString(argv[1]) != APPNAME)
    {
        return -1;
    }
#endif
    ///////////////////////////////////////////////////////
#ifndef MUSIC_GREATER_NEW
    MusicUtils::Core::setLocalCodec();
#endif
    ///////////////////////////////////////////////////////
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
    xml->readSysLoadConfig();

    QTranslator translator;
    translator.load(MusicUtils::Core::getLanguageName(
                M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndexChoiced).toInt()));
    a.installTranslator(&translator);

    MusicUtils::Core::checkCacheSize(
                M_SETTING_PTR->value(MusicSettingManager::DownloadCacheSizeChoiced).toInt()*MH_MB2B,
                M_SETTING_PTR->value(MusicSettingManager::DownloadLimitChoiced).toInt(),
                M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString());
    M_NETWORK_PTR->setBlockNetWork(
                M_SETTING_PTR->value(MusicSettingManager::CloseNetWorkChoiced).toInt());
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
