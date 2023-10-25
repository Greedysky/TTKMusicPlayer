#include "ttkrunapplication.h"
#include "musicapplication.h"
#include "musicruntimemanager.h"
#include "musicconfigobject.h"
#include "ttkdumper.h"
#include "ttkglobalhelper.h"
#include "ttkcommandline.h"
#include "ttkplatformsystem.h"

#ifdef Q_OS_UNIX
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
      TTKPlatformSystem platform;
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

    TTKRunApplication app(argc, argv);

    QCoreApplication::setOrganizationName(TTK_APP_NAME);
    QCoreApplication::setOrganizationDomain(TTK_APP_COME_NAME);
    QCoreApplication::setApplicationName(TTK_APP_NAME);

    if(app.isRunning())
    {
        TTK_INFO_STREAM("One app has already run");
        return -1;
    }

    MusicConfigObject config;
    config.valid();

    TTKDumper dumper(std::bind(cleanupCache));
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

    TTK::setApplicationFont();

    MusicApplication w;
    w.show();

    // parse command line args
    QStringList args;
    for(int i = 0; i < argc; ++i)
    {
        const QString &&arg = QString::fromLocal8Bit(argv[i]);
        if(!arg.endsWith(TTK_APP_EXE_NAME) && !arg.endsWith(TTK_SERVICE_EXE_NAME))
        {
            args << arg;
        }
    }

    if(args.count() == 2)
    {
        TTKCommandLineOption op1(MUSIC_OUTSIDE_OPEN);
        TTKCommandLineOption op2(MUSIC_OUTSIDE_LIST);

        TTKCommandLineParser parser;
        parser.addOption(op1);
        parser.addOption(op2);
        parser.process(args);

        if(parser.isSet(op1))
        {
            w.importSongsOutsideMode(parser.value(op1), true);
        }
        else if(parser.isSet(op2))
        {
            w.importSongsOutsideMode(parser.value(op2), false);
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
    const int ret = app.exec();
    cleanupCache();
    return ret;
}
