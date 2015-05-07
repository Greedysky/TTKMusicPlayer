#include "musicapplication.h"
#include <QApplication>
#include <QTranslator>
#include "core/musicobject.h"
#include "core/musicxmlconfigmanager.h"
//#include "core/musiclogger.h"
//#include <vld.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("QMusicPlayer");
    QCoreApplication::setOrganizationDomain("QMusicPlayer.com");
    QCoreApplication::setApplicationName("QMusicPlayer");

    MusicXMLConfigManager *xml = new MusicXMLConfigManager;
    xml->readXMLConfig();
    QTranslator translator;
    translator.load(MusicObject::getLanguageName(xml->readLanguageIndex()));
    a.installTranslator(&translator);
    delete xml;

    MusicObject::checkTheDirectoryExist();

//    mlogger("MusicApplication Begin");
    MusicApplication w;
    w.show();
//    mlogger("MusicApplication Stop");

    return a.exec();
}
