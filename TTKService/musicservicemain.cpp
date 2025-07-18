#include "ttkapplication.h"
#include "musicapplication.h"
#include "musicruntimemanager.h"
#include "musicconfigmodule.h"
#include "musicprocessmanager.h"
#include "ttkdumper.h"
#include "ttkdesktopscreen.h"
#include "ttkglobalinterface.h"

#ifdef Q_OS_LINUX
#  include <malloc.h>
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

static void loadApplicationScaleFactor()
{
#if TTK_QT_VERSION_CHECK(6,0,0)
   // do nothing
#elif TTK_QT_VERSION_CHECK(5,4,0)
#  if TTK_QT_VERSION_CHECK(5,12,0)
      QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
      QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#    if TTK_QT_VERSION_CHECK(5,14,0)
        QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#    endif
#  elif TTK_QT_VERSION_CHECK(5,6,0)
      const float dpi = TTKDesktopScreen::dotsPerInch() / 96.0;
      qputenv("QT_SCALE_FACTOR", QByteArray::number(dpi < 1.0 ? 1.0 : dpi));
#  endif
#endif
}

int main(int argc, char *argv[])
{
    loadApplicationScaleFactor();

    TTKApplication app(argc, argv);

    QCoreApplication::setOrganizationName(TTK_APP_NAME);
    QCoreApplication::setOrganizationDomain(TTK_APP_COME_NAME);
    QCoreApplication::setApplicationName(TTK_APP_NAME);

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

    MusicConfigModule config;
    config.valid();

    TTKDumper dumper(std::bind(cleanupCache));
    dumper.execute();

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

    TTK::setApplicationFont();

    MusicApplication w;
    w.show();

    MusicProcessServer server;
    server.execute(args);

#ifdef Q_OS_LINUX
    // unix mpris module
    MusicMPRISPlayer mpris;
    mpris.execute();
#endif

#ifdef Q_OS_LINUX
    // memory free
    mallopt(M_MMAP_THRESHOLD, 1024 * 1024);   // 1MB mmap
    mallopt(M_TRIM_THRESHOLD, 2 * 1024 * 1024); // 2MB brk
#endif
    return app.exec();
}
