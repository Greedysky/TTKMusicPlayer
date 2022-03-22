#include "archivereader.h"

#include <QProcess>
#include <QSettings>
#include <QDirIterator>
#include <qmmp/qmmp.h>

#define EXECUTE_PATH    (Qmmp::ttkPluginPath() + "/archive.tkx")

namespace archive {
static QString unpackPath()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Archive");
    const QString &unpack = settings.value("unpack_path").toString();
    settings.endGroup();
    return unpack;
}

static bool removeRecursively(const QString &dir)
{
    QDir dr(dir);
    if(!dr.exists())
    {
        return true;
    }

    bool success = true;
    const QString &dirPath = dr.path();
    // not empty -- we must empty it first
    QDirIterator di(dirPath, QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
    while(di.hasNext())
    {
        di.next();
        const QFileInfo &fin = di.fileInfo();
        const QString &filePath = di.filePath();

        bool ok;
        if(fin.isDir() && !fin.isSymLink())
        {
            ok = removeRecursively(filePath); // recursive
        }
        else
        {
            ok = QFile::remove(filePath);
            if(!ok)
            {
                // Read-only files prevent directory deletion on Windows, retry with Write permission.
                const QFile::Permissions permissions = QFile::permissions(filePath);
                if(!(permissions & QFile::WriteUser))
                {
                    ok = QFile::setPermissions(filePath, permissions | QFile::WriteUser) && QFile::remove(filePath);
                }
            }
        }

        if(!ok)
        {
            success = false;
        }
    }

    if(success)
    {
        success = dr.rmdir(dr.absolutePath());
    }
    return success;
}
}


QStringList ArchiveReader::list(const QString &path)
{
    QProcess process;
    QStringList args;
    args << "l" << path;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(EXECUTE_PATH, args);
    process.waitForFinished();

    const QString &unpack = archive::unpackPath();
    if(unpack.isEmpty())
    {
        return QStringList();
    }

    const QString &cache = unpack + path.section("/", -1) + "/";

    QStringList files;
    const QString &list = QString::fromLocal8Bit(process.readAllStandardOutput());
    for(const QString& file : list.split('\n'))
    {
        if(file.contains("....A"))
        {
            files << cache + file.section(" ", -1);
        }
    }
    return files;
}

bool ArchiveReader::unpack(const QString &path)
{
    const QString &unpack = archive::unpackPath();
    if(unpack.isEmpty())
    {
        return false;
    }

    const QString &cache = unpack + path.section("/", -1);
    const QString &hash = cache + "/hash.tkf";

    bool hasHashFile = false;
    const QByteArray &hashSize = QByteArray::number(QFileInfo(path).size());
    {
        QFile file(hash);
        if(file.open(QFile::ReadOnly))
        {
            hasHashFile = true;
            if(file.readAll() == hashSize)
            {
                qDebug("ArchiveReader: upack file dir not changed");
                file.close();
                return true;
            }
        }
    }

    {
        QDir dir;
        if(hasHashFile)
        {
            archive::removeRecursively(cache);
        }
        dir.mkpath(cache);

        QFile file(hash);
        if(!file.open(QFile::WriteOnly))
        {
            return false;
        }

        file.write(hashSize);
        file.close();
    }

    QProcess process;
    QStringList args;
    args << "x" << path << "-o" + cache;
    process.start(EXECUTE_PATH, args);
    process.waitForFinished();
    return true;
}
