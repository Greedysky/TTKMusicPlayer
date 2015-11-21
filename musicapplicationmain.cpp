#include "musicapplication.h"
#include "musicobject.h"
#include "musicxmlconfigmanager.h"
#include "musicnetworkthread.h"
#include "musiclogger.h"
#include "musictime.h"
#include "musicmessagebox.h"

#include <QApplication>
#include <QTranslator>
#ifndef MUSIC_QT_5
#  include <QTextCodec>
#endif
//#include <vld.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifndef MUSIC_QT_5
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    MusicObject::checkTheDirectoryExist();
    if(!MusicObject::checkTheFileExist())
    {
        MusicMessageBox(QObject::tr("QMusicPlayer"),
                        QObject::tr("Lack of necessary component files!")
                        ).exec();
        return -1;
    }//check file error!

    ///////////////////////////////////////////////////////
    M_LOGGERS("MusicApplication Begin");
    QCoreApplication::setOrganizationName("QMusicPlayer");
    QCoreApplication::setOrganizationDomain("QMusicPlayer.com");
    QCoreApplication::setApplicationName("QMusicPlayer");

    //detect the current network state
    M_NETWORK->start();

    M_LOGGERS("Load Translation");
    MusicXMLConfigManager *xml = new MusicXMLConfigManager;
    xml->readXMLConfig();
    QTranslator translator;
    translator.load(MusicObject::getLanguageName(xml->readLanguageIndex()));
    a.installTranslator(&translator);

    MusicTime::checkCacheSize(xml->readDownloadCacheSize()*1024*1024,
                              xml->readDownloadCacheLimit(), MUSIC_DOWNLOAD_AL);
    M_NETWORK->setBlockNetWork(xml->readCloseNetworkConfig());
    delete xml;
    M_LOGGERS("End load translation");

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
