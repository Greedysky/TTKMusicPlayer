#include <QApplication>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQmlApplicationEngine>

#include "core/ttkfilesearchcore.h"
#include "core/ttkmusicplaylist.h"
#include "core/ttkmusicplayer.h"
#include "core/ttkmusicutils.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    qmlRegisterType<TTKFileSearchCore>("TTKFileSearchCore", 1, 0, "TTKFileSearchCore");
    qmlRegisterType<TTKMusicPlaylist>("TTKMusicPlaylist", 1, 0, "TTKMusicPlaylist");
    qmlRegisterType<TTKMusicPlayer>("TTKMusicPlayer", 1, 0, "TTKMusicPlayer");

    QQmlApplicationEngine engine;
    QQmlContext *qmlContext = engine.rootContext();
    qmlContext->setContextProperty("TTK_UTILS", new TTKMusicUtils(qmlContext));

    engine.load(QUrl("qrc:/qmls/main.qml"));
    if(engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
