#include "musicconsolemodule.h"
#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QGuiApplication>
using TTKApplication = QGuiApplication;
#else
#  include <QApplication>
using TTKApplication = QApplication;
#endif

int main(int argc, char *argv[])
{
    TTKApplication app(argc, argv);

    QCoreApplication::setOrganizationName(APP_NAME);
    QCoreApplication::setOrganizationDomain(APP_COME_NAME);
    QCoreApplication::setApplicationName(APP_NAME);

    MusicConsoleModule object;
    return object.initialize(app);
}
