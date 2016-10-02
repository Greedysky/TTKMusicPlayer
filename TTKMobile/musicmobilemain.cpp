#include <QApplication>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQmlApplicationEngine>

#include "musicapplication.h"
#include "core/ttkfilesearchcore.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    //////////////////////////////////////////////////////////////
    qmlRegisterType<TTKFileSearchCore>("TTKFileSearchCore", 1, 0, "TTKFileSearchCore");

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
