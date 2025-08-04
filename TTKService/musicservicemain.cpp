#include "ttkapplication.h"
#include "musicapplication.h"
#include "musicruntimemanager.h"
#include "musicconfigmodule.h"
#include "musicprocessmanager.h"
#include "ttkinitialization.h"
#include "ttkprocess.h"

#ifdef Q_OS_LINUX
#  include "musicmprisplayer.h"
#endif

#include <QTranslator>

static void cleanupCache()
{
    QFile::remove(TTK_COLOR_FILE);
    QFile::remove(TTK_IMAGE_FILE);
    QFile::remove(TTK_RECORD_FILE);
    QFile::remove(TTK_RECORD_DATA_FILE);

    ///clean thirdparty process
    QStringList process;
    process << MAKE_TRANSFORM_NAME
            << MAKE_PLAYER_NAME
            << MAKE_GAIN_NAME;
    TTK::killProcessByName(process);
    TTK_INFO_STREAM("Application cache cleanup");
}


int main(int argc, char *argv[])
{
    TTK::loadApplicationScaleFactor();

    TTKApplication app(argc, argv);
    TTKInitialization ttk(cleanupCache);

    // parse command line args
    QStringList args;
    for(int i = 0; i < argc; ++i)
    {
        const QString &&arg = QString::fromLocal8Bit(argv[i]);
        if(!arg.endsWith(TTK_APP_NAME) && !arg.endsWith(TTK_SERVICE_NAME) && !arg.endsWith(TTK_APP_RUN_NAME) && !arg.endsWith(TTK_SERVICE_RUN_NAME))
        {
            args << arg;
        }
    }

    if(app.isRunning())
    {
        MusicProcessClient client;
        client.execute(args);

        TTK_INFO_STREAM("One app has already run");
        return EXIT_FAILURE;
    }

    ttk.generateFont();
    ttk.codecForLocale();
    ttk.execute(args);

    MusicConfigModule config;
    config.valid();

    MusicRunTimeManager manager;
    manager.execute();

    if(!manager.configVersionCheck())
    {
        config.reset();
    }

    for(const QString &ts : manager.translator())
    {
        QTranslator *translator = new QTranslator(&app);
        if(!translator->load(ts))
        {
            TTK_ERROR_STREAM("Load translation error: " << ts);
            delete translator;
            continue;
        }
        app.installTranslator(translator);
    }

    MusicApplication w;
    w.show();

    MusicProcessServer server;
    server.execute(args);

#ifdef Q_OS_LINUX
    // unix mpris module
    MusicMPRISPlayer mpris;
    mpris.execute();
#endif
    return app.exec();
}
