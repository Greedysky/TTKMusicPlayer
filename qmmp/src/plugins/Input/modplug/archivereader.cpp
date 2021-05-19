#include "archivereader.h"
#include <QProcess>
#include <unmo3.h>

ArchiveReader::ArchiveReader(QObject *parent)
    : QObject(parent)
{
    m_process = new QProcess(this);
}

ArchiveReader::~ArchiveReader()
{

}

bool ArchiveReader::isSupported(const QString &path)
{
    QString lPath = path.toLower();
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
    QString lPath = path.toLower();
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

bool ArchiveReader::mo3Check(const QString &path)
{
    return path.toLower().endsWith(".mo3");
}

bool ArchiveReader::mo3Decode(void **data, unsigned *len)
{
    return UNMO3_Decode(data, len, !UNMO3_DECODE_NOSAMPLES) == 0;
}

void ArchiveReader::mo3Free(void *data)
{
    UNMO3_Free(data);
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
