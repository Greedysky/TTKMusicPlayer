#include "musicupdatemainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("MLanguage/cn.ln");
    a.installTranslator(&translator);

    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    if(argc == 2)
    {
        MusicUpdateMainWindow w;
        w.show();
        w.setFilePath(argv[1], QString());
        return a.exec();
    }
    return 0;
}
