#include <QApplication>
#include <QQmlContext>
#include <QTranslator>
#include <QQmlApplicationEngine>

#include "musicapplication.h"
#include "core/ttkmusicutils.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    TTKMusicUtils().checkTheFileNeededExist();

    QTranslator translator;
    translator.load("MLanguage/TTKMobile.ln");
    app.installTranslator(&translator);
    //////////////////////////////////////////////////////////////
    QQmlApplicationEngine engine;
    QQmlContext *qmlContext = engine.rootContext();
    if(qmlContext)
    {
        new MusicApplication(qmlContext);
    }

    engine.load(QUrl("qrc:/qmls/main.qml"));
    if(engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
