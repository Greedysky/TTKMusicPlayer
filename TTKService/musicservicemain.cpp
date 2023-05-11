#include "musicapplication.h"
#include "musicruntimemanager.h"
#include "musicconfigobject.h"
#include "musicplatformmanager.h"
#include "ttkglobalhelper.h"
#include "ttkdumper.h"

#ifdef Q_OS_UNIX
#  include <malloc.h>
#  include "musicmprisplayer.h"
#endif

#include <QTranslator>
#include <QApplication>

static void cleanAppicationCache()
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
}

static void loadAppScaledFactor(int argc, char *argv[])
{
#if TTK_QT_VERSION_CHECK(6,0,0)
   // do nothing
#elif TTK_QT_VERSION_CHECK(5,4,0)
#  if TTK_QT_VERSION_CHECK(5,12,0)
      QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
      QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#    if TTK_QT_VERSION_CHECK(5,14,0)
        QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#    endif
#  elif TTK_QT_VERSION_CHECK(5,6,0)
     MusicPlatformManager platform;
     const float dpi = platform.logicalDotsPerInch() / 96.0;
     qputenv("QT_SCALE_FACTOR", QByteArray::number(dpi < 1.0 ? 1.0 : dpi));
#  else
//      qputenv("QT_DEVICE_PIXEL_RATIO", "auto");
#  endif
#endif
    Q_UNUSED(argc);
    Q_UNUSED(argv);
}

int main(int argc, char *argv[])
{
    loadAppScaledFactor(argc, argv);

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(APP_NAME);
    QCoreApplication::setOrganizationDomain(APP_COME_NAME);
    QCoreApplication::setApplicationName(APP_NAME);

    MusicConfigObject config;
    config.valid();

    TTKDumper dumper(std::bind(cleanAppicationCache));
    dumper.run();

    MusicRunTimeManager manager;
    manager.run();
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

    TTK::setApplicationFont(&app);

    MusicApplication w;
    w.show();

    // parse command line args
    QStringList args;
    for(int i = 0; i < argc; ++i)
    {
        const QString &&arg = QString::fromLocal8Bit(argv[i]);
        if(!arg.endsWith(APP_NAME) && !arg.endsWith(SERVICE_NAME))
        {
            args << arg;
        }
    }

    if(args.count() == 2)
    {
        if(args[0] == MUSIC_OUTSIDE_OPEN)
        {
            w.musicImportSongsPathOutside({args[1]}, true);
        }
        else if(args[0] == MUSIC_OUTSIDE_LIST)
        {
            w.musicImportSongsPathOutside({args[1]}, false);
        }
    }

#ifdef Q_OS_UNIX
    // unix mpris module
    MusicMPRISPlayer mpris;
    mpris.run();
    // memory free
    mallopt(M_MMAP_THRESHOLD, 1024 * 1024);   // 1MB mmap
    mallopt(M_TRIM_THRESHOLD, 2 * 1024 * 1024); // 2MB brk
#endif
    return app.exec();
}
