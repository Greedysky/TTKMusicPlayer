#include "musicrunapplication.h"
#include "musicrunobject.h"

int main(int argc, char *argv[])
{
    MusicRunApplication app(argc, argv);
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
