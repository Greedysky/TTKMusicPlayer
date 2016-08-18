#include "musicserviceapplication.h"
#include "musicserviceobject.h"

int main(int argc, char *argv[])
{
    MusicServiceApplication app(argc, argv);
    if(app.isRunning())
    {
        return -1;
    }

    Q_INIT_RESOURCE(MusicService);

    MusicServiceObject object;
    object.checkValid();
    object.run(argc, argv);

    return app.exec();
}
