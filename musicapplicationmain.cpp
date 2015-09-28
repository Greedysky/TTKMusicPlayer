#include "musicapplication.h"
#include "musicobject.h"
#include "musicxmlconfigmanager.h"
#include "musicnetworkthread.h"
#include "musicsettingmanager.h"
#include "musiclogger.h"

#include <QDesktopWidget>
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
    translator.load(MusicObject::getLanguageName(xml->readLanguageIndex()));
    a.installTranslator(&translator);
    delete xml;
    M_LOOGERS("End load translation");

    //detect the current network state
    M_NETWORK->start();

    MusicObject::checkTheDirectoryExist();

    QWidget *widget = QApplication::desktop();
    M_SETTING->setValue(MusicSettingManager::ScreenSize, widget->size());

    MusicApplication w;
    w.show();
    w.move((widget->width() - w.width())/2, (widget->height() - w.height())/2);

    return a.exec();
}
