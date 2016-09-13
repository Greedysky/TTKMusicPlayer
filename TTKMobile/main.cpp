#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    int fontId = QFontDatabase::addApplicationFont(":/fonts/iconfont.ttf");
    QFontDatabase::applicationFontFamilies(fontId);

    return app.exec();
}
