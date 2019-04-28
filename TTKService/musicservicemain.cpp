#include "musicapplication.h"
#include "musicruntimemanager.h"
#include "musicotherdefine.h"
#include "ttkdumper.h"

#include <QScreen>
#include <QTranslator>
#include <QApplication>

#define TTK_DEBUG_CASE

void loadDXcbPlugin(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    #if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
      Q_UNUSED(argc);
      Q_UNUSED(argv);
      QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #else
      QApplication a(argc, argv);
      qputenv("QT_DEVICE_PIXEL_RATIO", "auto");
      QScreen *screen = QApplication::primaryScreen();
      const qreal dpi = screen->logicalDotsPerInch() / 96;
      qputenv("QT_SCALE_FACTOR", QByteArray::number(dpi));
      Q_UNUSED(a);
    #endif
#else
    Q_UNUSED(argc);
    Q_UNUSED(argv);
#endif
}

int main(int argc, char *argv[])
{
    loadDXcbPlugin(argc, argv);
    QApplication a(argc, argv);
#if !defined TTK_DEBUG_CASE && !defined Q_OS_UNIX
    if(argc <= 1 || QString(argv[1]) != APP_NAME)
    {
        return -1;
    }
#endif
    //
    QCoreApplication::setOrganizationName(APP_NAME);
    QCoreApplication::setOrganizationDomain(APP_COME_NAME);
    QCoreApplication::setApplicationName(APP_NAME);

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
            w.musicImportSongsSettingPathOuter(QStringList() << QString::fromLocal8Bit(argv[3]), true);
        }
        if(QString::fromLocal8Bit(argv[2]) == MUSIC_OUTER_LIST)
        {
            w.musicImportSongsSettingPathOuter(QStringList() << QString::fromLocal8Bit(argv[3]), false);
        }
        if(QString::fromLocal8Bit(argv[2]) == MUSIC_OUTER_RADIO)
        {
            w.radioExecuteOuter(argv[3]);
        }
    }

    return a.exec();
}
