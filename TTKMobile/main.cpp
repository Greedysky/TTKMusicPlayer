#include <QApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    int fontId = QFontDatabase::addApplicationFont(":/fonts/iconfont.ttf");
    QFontDatabase::applicationFontFamilies(fontId);

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/qmls/qmls/main.qml"));
    if(engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
