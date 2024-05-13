#include <QApplication>
#include "toolfm.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(TTK_APP_NAME);
    QCoreApplication::setOrganizationDomain(TTK_APP_COME_NAME);
    QCoreApplication::setApplicationName(TTK_APP_NAME);

    ToolFM w;
    w.startToRequest();

    return app.exec();
}
