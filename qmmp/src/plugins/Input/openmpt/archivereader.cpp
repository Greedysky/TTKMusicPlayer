#include "archivereader.h"

#include <QProcess>
#include <qmmp/qmmp.h>

#define EXECUTE_PATH    (Qmmp::ttkPluginPath() + "/archive.tkx")

QStringList ArchiveReader::archiveFilters()
{
    QStringList path;
    path << "*.mdr" << "*.s3r" << "*.xmr" << "*.itr";
    path << "*.mdz" << "*.s3z" << "*.xmz" << "*.itz";
    path << "*.mdgz" << "*.s3gz" << "*.xmgz" << "*.itgz";
    path << "*.mdbz" << "*.s3bz" << "*.xmbz" << "*.itbz";
    return path;
}

bool ArchiveReader::isSupported(const QString &path)
{
    if(!QFile::exists(EXECUTE_PATH))
    {
        return false;
    }

    const QString &filePath = path.toLower();
    for(const QString &suffix : archiveFilters())
    {
        if(filePath.endsWith(suffix.mid(1, suffix.length())))
        {
            return true;
        }
    }
    return false;
}

QByteArray ArchiveReader::unpack(const QString &path)
{
    QProcess process;
    QStringList args;
    args << "-so" << "e" << path;
    process.start(EXECUTE_PATH, args);
    process.waitForFinished();
    return process.readAllStandardOutput();
}
