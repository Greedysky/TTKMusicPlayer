#include <QCoreApplication>
#include "musicinitobject.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setOrganizationName(APP_NAME);
    QCoreApplication::setOrganizationDomain(APP_COME_NAME);
    QCoreApplication::setApplicationName(APP_NAME);

    MusicInitObject object;
    object.init();
    qDebug() << "Parameter Init success!";

//    return app.exec();
    return 0;
}
