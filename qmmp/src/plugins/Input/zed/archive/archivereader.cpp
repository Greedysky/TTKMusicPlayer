#include "archivereader.h"
#if defined Q_OS_UNIX || defined Q_CC_GNU
#  include <unistd.h>
#endif
#include <qmmp/qmmp.h>
#include <QProcess>
#include <QSettings>
#include <QDirIterator>
#include <QApplication>

#define ARCHIVE_PATH    (Qmmp::ttkPluginPath() + "/archive.tkx")

namespace archive {
static QString unpackPath()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Archive");
    const QString &unpack = settings.value("unpack_path").toString();
    settings.endGroup();
    return unpack;
}
}


QStringList ArchiveReader::list(const QString &path)
{
    QProcess process;
    QStringList args;
    args << "l" << path;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(ARCHIVE_PATH, args);
    process.waitForFinished();

    const QString &unpack = archive::unpackPath();
    if(unpack.isEmpty())
    {
        return {};
    }

    const QString &cache = unpack + QFileInfo(path).fileName() + "/";

    QStringList files;
    const QByteArray &buffer = process.readAllStandardOutput();
    const QString &list = QString::fromLocal8Bit(buffer.constData(), buffer.length());
#ifdef Q_OS_WIN
    for(QString &file : list.split("\r\n"))
    {
        file.replace("\\", "/");
#else
    for(const QString &file : list.split('\n'))
    {
#endif
        if(file.contains("....A"))
        {
            files << cache + file.section(" ", -1);
        }
    }
    return files;
}

bool ArchiveReader::unpack(const QString &path)
{
    if(!QFile::exists(ARCHIVE_PATH) || path.isEmpty())
    {
        return false;
    }

    const QString &unpack = archive::unpackPath();
    if(unpack.isEmpty())
    {
        return false;
    }

    const QString &cache = unpack + QFileInfo(path).fileName();
    const QString &hash = cache + "/hash.tkf";

    const qint64 size = QFileInfo(path).size();
    {
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        settings.beginGroup("Archive");
        const int maxSize = settings.value("max_size", 64).toInt();
        settings.endGroup();

        if(size > maxSize * 1024 * 1024)
        {
            qDebug("ArchiveReader: file size is large");
            return false;
        }
    }

    bool hasHashFile = false;
    const QByteArray &hashSize = QByteArray::number(size);
    {
        QFile file(hash);
        if(file.open(QIODevice::ReadOnly))
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
            removeRecursively(cache);
        }
        dir.mkpath(cache);

        QFile file(hash);
        if(!file.open(QIODevice::WriteOnly))
        {
            return false;
        }

        file.write(hashSize);
        file.close();
    }

    QProcess process;
    QStringList args;
    args << "x" << path << "-o" + cache;
    process.start(ARCHIVE_PATH, args);
    while(process.state() != QProcess::NotRunning)
    {
        qApp->processEvents();
        usleep(10 * 1000);
    }
    return true;
}

bool ArchiveReader::removeRecursively(const QString &dir)
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

        bool ok = false;
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


QStringList A7zipReader::filters()
{
    QStringList filter;
    filter << "*.7z" << "*.xz" << "*.bz2" << "*.gz" << "*.tar" << "*.zip" << "*.wim";
    filter << "*.ar" << "*.arj";
    filter << "*.cab" << "*.chm" << "*.cpio" << "*.cramfs";
    filter << "*.dmg";
    filter << "*.ext";
    filter << "*.fat";
    filter << "*.gpt";
    filter << "*.hfs";
    filter << "*.ihex" << "*.iso";
    filter << "*.lzh" << "*.lzma";
    filter << "*.mbr" << "*.msi";
    filter << "*.nsis" << "*.ntfs";
    filter << "*.qcow2";
    filter << "*.rar" << "*.rpm";
    filter << "*.squashfs";
    filter << "*.udf" << "*.uefi";
    filter << "*.vdi" << "*.vhd" << "*.vmdk";
    filter << "*.xar";
    filter << "*.z";
    return filter;
}

QStringList RSNReader::filters()
{
    QStringList filter;
    filter << "*.rsn" << "*.rps" << "*.rdc" << "*.rds" << "*.rgs" << "*.r64";
    return filter;
}
