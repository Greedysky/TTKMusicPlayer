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
    TTK::initialize(TTK::Attribute::All);

#ifndef Q_OS_LINUX
    TTKApplication app(argc, argv);
#else
    // Fixes this: https://bugreports.qt.io/browse/QTBUG-96214
    constexpr const char *WEB_SECURITY = "--disable-web-security";
    constexpr const char *SECCOMP_SECURITY = "--disable-seccomp-filter-sandbox";
    std::vector<char*> newArgv(argv, argv + argc);
    newArgv.push_back(const_cast<char*>(WEB_SECURITY));
    newArgv.push_back(const_cast<char*>(SECCOMP_SECURITY));
    newArgv.push_back(nullptr);
    int newArgc = argc + 2;
    TTKApplication app(newArgc, newArgv.data());
#endif
    TTKInitialization ttk(cleanupCache);

    // parse command line args
    QStringList args;
    for(int i = 0; i < argc; ++i)
    {
        const QString &arg = QString::fromLocal8Bit(argv[i]);
        if(ttk.argumentValid(arg))
        {
            args << arg;
        }
    }

    ttk.execute(args);

    if(app.isRunning())
    {
        MusicProcessClient client;
        client.execute(args);

        TTK_INFO_STREAM("One app has already run");
        return EXIT_FAILURE;
    }

    MusicConfigModule config;
    config.execute();

    MusicRunTimeManager manager;
    manager.execute();

    ttk.generateFont();
    ttk.codecForLocale();

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
