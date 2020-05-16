#include <QCoreApplication>
#include "musicconfigobject.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setOrganizationName(APP_NAME);
    QCoreApplication::setOrganizationDomain(APP_COME_NAME);
    QCoreApplication::setApplicationName(APP_NAME);

    MusicConfigObject object;
    object.initialize();
    TTK_LOGGER_INFO("Parameter Init success!");

    return 0;
}
