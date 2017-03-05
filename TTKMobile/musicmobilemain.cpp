#include <QApplication>
#include <QQmlContext>
#include <QTranslator>
#include <QQmlApplicationEngine>

#include "musicobject.h"
#include "musicapplication.h"
#include "ttkmusicconfigmanager.h"
#include "ttkmusicutils.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    //////////////////////////////////////////////////////////////
    TTKMusicConfigManager *xml = new TTKMusicConfigManager;
    xml->readXMLConfig(COFIGPATH_FULL);
    TTKMusicUtils utils;
    if(xml->readNeedUpdateConfig())
    {
        utils.removeDir(APPDATA_DIR_FULL);
    }
    utils.checkTheFileNeededExist();
    delete xml;
    //////////////////////////////////////////////////////////////
    QTranslator translator;
    translator.load(LANGUAGE_DIR_FULL + "TTKMobile.ln");
    app.installTranslator(&translator);
    //////////////////////////////////////////////////////////////
    QQmlApplicationEngine engine;
    QQmlContext *qmlContext = engine.rootContext();
    if(qmlContext)
    {
        new MusicApplication(qmlContext);
    }

    engine.load(QUrl("qrc:/MobileWidgets/main.qml"));
    if(engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
