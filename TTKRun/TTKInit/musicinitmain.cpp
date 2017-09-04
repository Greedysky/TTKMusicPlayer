#include <QCoreApplication>
#include "musicinitobject.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TTKMusicPlayer");
    QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com");
    QCoreApplication::setApplicationName("TTKMusicPlayer");

    MusicInitObject object;
    object.init();
    qDebug() << "Parameter Init success!";

//    return app.exec();
    return 0;
}
