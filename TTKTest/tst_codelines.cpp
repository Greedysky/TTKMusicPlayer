#include "tst_codelines.h"

#include <QDir>
#include <QMap>
#include <QDebug>
#include <QFileInfo>
#include <functional>
#ifdef TTK_GREATER_NEW
#  include <QtConcurrent/QtConcurrent>
#else
#  include <QtConcurrentRun>
#endif

void foreachFileFromDirectory(const QDir &directory, const std::function<void(const QFileInfo &)> &each, const bool &recursion)
{
    for(const auto &now: directory.entryInfoList(QDir::Files))
    {
        each(now);
    }

    if(recursion)
    {
        for(const auto &now: directory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot))
        {
            foreachFileFromDirectory(now.filePath(), each, recursion);
        }
    }
}

void CodeLinesTest::initTestCase()
{

}

void CodeLinesTest::codeLines()
{
    int fileCount = 0, lineCount = 0;

    const QString currentPath = "../";
    if(currentPath.isEmpty())
    {
        return;
    }

    QSet<QString> availableSuffixs;
    availableSuffixs << "h" << "c" << "cc" << "cp" << "cpp" << "hpp" << "inc"
                     << "i" << "ii" << "m" << "qml" << "pro" << "pri" << "prf" << "prl";

    QMap<QString, int> categorys;

    QEventLoop eventLoop;
    QtConcurrent::run( [&]()
    {
        foreachFileFromDirectory( { currentPath }, [&](const QFileInfo &info)
        {
            QString suffix = info.suffix().toLower();
            if(suffix.isEmpty())
            {
               suffix = "other";
            }
            categorys.insert(suffix, categorys[suffix] + 1);

            QFile file(info.filePath());
            if(!file.open(QIODevice::ReadOnly))
            {
                return;
            }

            fileCount++;

            const auto &&fileAllData = file.readAll();
            if(fileAllData.isEmpty())
            {
                return;
            }

            if(availableSuffixs.contains(suffix))
            {
                lineCount += fileAllData.count('\n') + 1;
            }
            file.close();
        }, true);

        eventLoop.quit();
    } );
    eventLoop.exec();

    qDebug() << "All File Count " << fileCount;
    qDebug() << "All Meet The Requirements Line Count " << lineCount;
    foreach(const QString &key, categorys.keys())
    {
        qDebug() << QString(".%1 Type  All count %2").arg(key).arg(categorys[key]);
    }
}

void CodeLinesTest::cleanupTestCase()
{

}
