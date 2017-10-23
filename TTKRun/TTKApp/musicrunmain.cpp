#include "musicrunapplication.h"
#include "musicrunobject.h"
#include "musicobject.h"

int main(int argc, char *argv[])
{
    MusicRunApplication app(argc, argv);

    QCoreApplication::setOrganizationName(APPNAME);
    QCoreApplication::setOrganizationDomain(APPCOME);
    QCoreApplication::setApplicationName(APPNAME);

    if(app.isRunning())
    {
        qDebug() << "One app has already run!";
        return -1;
    }

    MusicRunObject object;
    object.checkValid();
    object.run(argc, argv);

    return app.exec();
}
