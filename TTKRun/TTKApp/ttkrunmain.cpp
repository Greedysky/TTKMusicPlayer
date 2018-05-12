#include "ttkrunapplication.h"
#include "ttkrunobject.h"
#include "musicobject.h"

int main(int argc, char *argv[])
{
    TTKRunApplication app(argc, argv);

    QCoreApplication::setOrganizationName(APPNAME);
    QCoreApplication::setOrganizationDomain(APPCOME);
    QCoreApplication::setApplicationName(APPNAME);

    if(app.isRunning())
    {
        qDebug() << "One app has already run!";
        return -1;
    }

    TTKRunObject object;
    object.checkValid();
    object.run(argc, argv);

    return app.exec();
}
