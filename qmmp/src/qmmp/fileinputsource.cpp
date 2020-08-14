#include <QFile>
#include "fileinputsource_p.h"

FileInputSource::FileInputSource(const QString &url, QObject *parent)
    : InputSource(url,parent)
{
    m_file = new QFile(url, this);
}

QIODevice *FileInputSource::ioDevice() const
{
    return m_file;
}

bool FileInputSource::initialize()
{
    emit ready();
    return true;
}

bool FileInputSource::isReady() const
{
    return m_file->isOpen();
}
