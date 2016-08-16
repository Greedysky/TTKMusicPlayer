#include "musicserviceapplication.h"
#include "musicserviceobject.h"

int main(int argc, char *argv[])
{
    MusicServiceApplication app(argc, argv);
    if(app.isRunning())
    {
        return -1;
    }

    MusicServiceObject object;
    object.run();

    return app.exec();
}
