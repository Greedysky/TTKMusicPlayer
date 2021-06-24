#include "archivereader.h"

ArchiveReader::ArchiveReader(QObject *parent)
    : QObject(parent)
{
    m_process = new QProcess(this);
}

ArchiveReader::~ArchiveReader()
{
    delete m_process;
}

bool ArchiveReader::isSupported(const QString &path)
{
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

QByteArray ArchiveReader::unpack(const QString &path)
{
    const QString &lPath = path.toLower();
    if(path.endsWith(".mdz") ||
            lPath.endsWith(".s3z") ||
            lPath.endsWith(".xmz") ||
            lPath.endsWith(".itz"))
        return unzip(path);
    else if(lPath.endsWith(".mdgz") ||
             lPath.endsWith(".s3gz") ||
             lPath.endsWith(".xmgz") ||
             lPath.endsWith(".itgz"))
        return gunzip(path);
    else if(lPath.endsWith(".mdbz"))
        return bunzip2(path);

    return QByteArray();
}

QByteArray ArchiveReader::unzip(const QString &path)
{
    QStringList args;
    args << "-p" << path;
    m_process->start("unzip", args);
    m_process->waitForFinished();
    return m_process->readAllStandardOutput();
}

QByteArray ArchiveReader::gunzip(const QString &path)
{
    QStringList args;
    args << "-c" << path;
    m_process->start("gunzip", args);
    m_process->waitForFinished();
    return m_process->readAllStandardOutput();
}

QByteArray ArchiveReader::bunzip2(const QString &path)
{
    QStringList args;
    args << "-c" << path;
    m_process->start("bunzip2", args);
    m_process->waitForFinished();
    return m_process->readAllStandardOutput();
}
