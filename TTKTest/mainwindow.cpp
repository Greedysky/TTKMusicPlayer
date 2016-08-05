#include "mainwindow.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>

// C++ lib import
#include <functional>

// Qt lib import
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#if (defined Q_OS_MAC) || (defined __MINGW32__)
#   include "utime.h"
#endif
#include <QMap>


void foreachFileFromDirectory(const QDir &directory, const std::function<void(const QFileInfo &)> &each, const bool &recursion)
{
    for (const auto &now: directory.entryInfoList(QDir::Files))
    {
        each(now);
    }

    if (recursion)
    {
        for (const auto &now: directory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot))
        {
            foreachFileFromDirectory(now.filePath(), each, recursion);
        }
    }
}


void MainWindow::init()
{
    int fileCount = 0;
    int lineCount = 0;

    QString currentPath = QFileDialog::getExistingDirectory( nullptr,
                                                          "Please choose code dir",
                                                          QStandardPaths::writableLocation( QStandardPaths::DesktopLocation ) );
    if( currentPath.isEmpty() )
    {
        return;
    }

    QSet<QString> availableSuffixs;

    availableSuffixs << "h" << "c" << "cc" << "cp" << "cpp" << "hpp" << "inc"
                     << "i" << "ii" << "m" << "qml" << "pro" << "pri" << "prf" << "prl";

    QMap<QString, int> fileSomgth;

    QEventLoop eventLoop;
    QtConcurrent::run( [ & ]()
    {

        foreachFileFromDirectory( { currentPath }, [ & ](const QFileInfo &info)
        {
            QString sds = info.suffix().toLower();
            if(sds.isEmpty())
            {
               sds = "Other";
            }
            fileSomgth.insert(sds, fileSomgth.value(sds) + 1);

            QFile file( info.filePath() );
            if ( !file.open(QIODevice::ReadOnly) ) { return; }

            fileCount++;

            const auto &&fileAllData = file.readAll();

            if ( fileAllData.isEmpty() ) { return; }

            if(availableSuffixs.contains(info.suffix().toLower()))
            {
             lineCount += fileAllData.count('\n') + 1;
            }
            file.close();
        }, true);

        eventLoop.quit();
    } );

    eventLoop.exec();
    qDebug() << fileCount << lineCount << fileSomgth;

}
