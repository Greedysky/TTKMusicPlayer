#include <QApplication>
#include "toolsurl.h"
#include "musicobject.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(APP_NAME);
    QCoreApplication::setOrganizationDomain(APP_COME_NAME);
    QCoreApplication::setApplicationName(APP_NAME);

    ToolsUrl w;
    w.show();

    return app.exec();
}
