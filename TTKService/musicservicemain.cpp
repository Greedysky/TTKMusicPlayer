#include "musicapplication.h"
#include "musicruntimemanager.h"
#include "musicconfigobject.h"
#include "musicplatformmanager.h"
#include "ttkdumper.h"

#include <QTranslator>
#include <QApplication>

#define TTK_DEBUG

void loadAppScaledFactor(int argc, char *argv[])
{
#if TTK_QT_VERSION_CHECK(5,4,0)
    #if TTK_QT_VERSION_CHECK(5,12,0)
      QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #elif TTK_QT_VERSION_CHECK(5,6,0)
      MusicPlatformManager platform;
      const float dpi = platform.getLogicalDotsPerInch() / 96.0;
      qputenv("QT_SCALE_FACTOR", QByteArray::number(dpi < 1.0 ? 1.0 : dpi));
    #else
      qputenv("QT_DEVICE_PIXEL_RATIO", "auto");
    #endif
#endif
    Q_UNUSED(argc);
    Q_UNUSED(argv);
}

int main(int argc, char *argv[])
{
    loadAppScaledFactor(argc, argv);
    //
    QApplication a(argc, argv);
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

    MusicConfigObject object;
    object.checkValid();

    TTKDumper dumper;
    dumper.run();

    MusicRunTimeManager manager;
    manager.run();

    QTranslator translator;
    translator.load(manager.translator());
    a.installTranslator(&translator);

    MusicApplication w;
    w.show();
    //

    if(argc == 4)
    {
        if(QString::fromLocal8Bit(argv[2]) == MUSIC_OUTER_OPEN)
        {
            w.musicImportSongsPathOuter(QStringList() << QString::fromLocal8Bit(argv[3]), true);
        }
        if(QString::fromLocal8Bit(argv[2]) == MUSIC_OUTER_LIST)
        {
            w.musicImportSongsPathOuter(QStringList() << QString::fromLocal8Bit(argv[3]), false);
        }
        if(QString::fromLocal8Bit(argv[2]) == MUSIC_OUTER_RADIO)
        {
            w.radioExecuteOuter(argv[3]);
        }
    }

    return a.exec();
}
