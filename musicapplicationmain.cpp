#include "musicapplication.h"
#include "musicobject.h"
#include "musicxmlconfigmanager.h"
#include "musicnetworkthread.h"
#include "musiclogger.h"

#include <QApplication>
#include <QTranslator>
//#include <vld.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    M_LOOGERS("MusicApplication Begin");
    QCoreApplication::setOrganizationName("QMusicPlayer");
    QCoreApplication::setOrganizationDomain("QMusicPlayer.com");
    QCoreApplication::setApplicationName("QMusicPlayer");

    M_LOOGERS("Load Translation");
    MusicXMLConfigManager *xml = new MusicXMLConfigManager;
    xml->readXMLConfig();

    QTranslator translator;
    qDebug()<<MusicObject::getLanguageName(xml->readLanguageIndex());
    translator.load(MusicObject::getLanguageName(xml->readLanguageIndex()));
    a.installTranslator(&translator);
    delete xml;
    M_LOOGERS("End load translation");

    //detect the current network state
    M_NETWORK->start();

    MusicObject::checkTheDirectoryExist();
    MusicApplication w;
    w.show();

    if(argc == 3 && QString(argv[1]) == "-path" )
    {
        w.musicImportSongsSettingPath(QStringList() << argv[2]);
    }

    return a.exec();
}
