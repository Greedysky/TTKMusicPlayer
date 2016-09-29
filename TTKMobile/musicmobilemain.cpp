#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "Core/ttkfilesearchcore.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    qmlRegisterType<TTKFileSearchCore>("TTKFileSearchCore", 1, 0, "TTKFileSearchCore");

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/qmls/main.qml"));
    if(engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
