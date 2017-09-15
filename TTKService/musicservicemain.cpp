#include "musicapplication.h"
#include "musicruntimemanager.h"
#include "musicnumberdefine.h"
#include "ttkdumper.h"

#include <QTimer>
#include <QTranslator>
#include <QApplication>

#define MUSIC_DEBUG

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if !defined MUSIC_DEBUG && !defined Q_OS_UNIX
    if(argc <= 1 || QString(argv[1]) != APPNAME)
    {
        return -1;
    }
#endif
    ///////////////////////////////////////////////////////
    QCoreApplication::setOrganizationName(APPNAME);
    QCoreApplication::setOrganizationDomain(APPCOME);
    QCoreApplication::setApplicationName(APPNAME);

    TTKDumper dumper;
    dumper.run();

    MusicRunTimeManager manager;
    manager.run();

    QTranslator translator;
    translator.load(manager.translator());
    a.installTranslator(&translator);

    MusicApplication w;
    w.show();
    ///////////////////////////////////////////////////////

    if(argc == 4)
    {
        if( QString(argv[1]) == "-Open" )
        {
            w.musicImportSongsSettingPath(QStringList() << argv[2]);
            QTimer::singleShot(MT_MS, &w, SLOT(musicImportPlay()));
        }
        if( QString(argv[1]) == "-List" )
        {
            w.musicImportSongsSettingPath(QStringList() << argv[2]);
        }
    }

    return a.exec();
}
