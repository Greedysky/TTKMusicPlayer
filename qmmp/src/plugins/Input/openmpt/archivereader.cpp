#include "archivereader.h"

#include <QFile>
#include <QProcess>

#define EXECUTE_PATH    (Qmmp::pluginPath() + "/../GPlugins/archive.tkx")

bool ArchiveReader::isSupported(const QString &path) const
{
    if(!QFile::exists(EXECUTE_PATH))
    {
        return false;
    }

    const QString &lPath = path.toLower();
    return lPath.endsWith(".mdz") ||
           lPath.endsWith(".s3z") ||
           lPath.endsWith(".xmz") ||
           lPath.endsWith(".itz") ||
           lPath.endsWith(".mdgz") ||
           lPath.endsWith(".s3gz") ||
           lPath.endsWith(".xmgz") ||
           lPath.endsWith(".itgz") ||
           lPath.endsWith(".mdbz") ||
           lPath.endsWith(".s3bz") ||
           lPath.endsWith(".xmbz") ||
           lPath.endsWith(".itbz");
}

QByteArray ArchiveReader::unpack(const QString &path) const
{
    QProcess process;
    QStringList args;
    args << "e" << path.toLower();
    process.start(EXECUTE_PATH, args);
    process.waitForFinished();
    return process.readAllStandardOutput();
}
