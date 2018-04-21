#include <QApplication>
#include "toolsskin.h"
#include "musicobject.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(APPNAME);
    QCoreApplication::setOrganizationDomain(APPCOME);
    QCoreApplication::setApplicationName(APPNAME);

    ToolsSkin w;
    w.show();

    return app.exec();
}
