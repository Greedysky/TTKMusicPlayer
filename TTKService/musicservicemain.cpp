#include "musicapplication.h"
#include "musicruntimemanager.h"
#include "musicconfigobject.h"
#include "musicplatformmanager.h"
#include "ttkdumper.h"
#ifdef Q_OS_UNIX
#  include "musicmprisplayer.h"
#endif

#include <QTranslator>
#include <QApplication>

#define TTK_DEBUG

void loadAppScaledFactor(int argc, char *argv[])
{
#if TTK_QT_VERSION_CHECK(5,4,0)
#  if TTK_QT_VERSION_CHECK(6,0,0)
     // do nothing
#  elif TTK_QT_VERSION_CHECK(5,12,0)
     QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#  elif TTK_QT_VERSION_CHECK(5,6,0)
     MusicPlatformManager platform;
     const float dpi = platform.logicalDotsPerInch() / 96.0;
     qputenv("QT_SCALE_FACTOR", QByteArray::number(dpi < 1.0 ? 1.0 : dpi));
#  else
//     qputenv("QT_DEVICE_PIXEL_RATIO", "auto");
#  endif
#endif
    Q_UNUSED(argc);
    Q_UNUSED(argv);
}

int main(int argc, char *argv[])
{
    loadAppScaledFactor(argc, argv);
    //
    QApplication app(argc, argv);
#if !defined TTK_DEBUG && !defined Q_OS_UNIX
    if(argc <= 1 || QString(argv[1]) != APP_NAME)
    {
        return -1;
    }
#endif
    //
    QCoreApplication::setOrganizationName(APP_NAME);
    QCoreApplication::setOrganizationDomain(APP_COME_NAME);
    QCoreApplication::setApplicationName(APP_NAME);

    MusicConfigObject config;
    config.valid();

    TTKDumper dumper;
    dumper.run();

    MusicRunTimeManager manager;
    manager.run();
    if(!manager.configVersionCheck())
    {
        config.reset();
    }

    QTranslator translator;
    if(!translator.load(manager.translator()))
    {
        TTK_LOGGER_ERROR("Load translation error");
    }
    app.installTranslator(&translator);

    MusicApplication w;
    w.show();

#ifdef Q_OS_WIN
    if(argc == 4)
    {
        const QString &data = QString::fromLocal8Bit(argv[3]);
        if(data == MUSIC_OUTSIDE_OPEN)
        {
            w.musicImportSongsPathOutside({QString::fromLocal8Bit(argv[4])}, true);
        }
        else if(data == MUSIC_OUTSIDE_LIST)
        {
            w.musicImportSongsPathOutside({QString::fromLocal8Bit(argv[4])}, false);
        }
        else if(data == MUSIC_OUTSIDE_RADIO)
        {
            w.radioExecuteOutside(argv[4]);
        }
    }
#elif defined Q_OS_UNIX
    MusicMPRISPlayer mpris;
    mpris.run();
#endif
    return app.exec();
}
