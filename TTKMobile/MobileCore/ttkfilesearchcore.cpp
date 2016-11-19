#include "ttkfilesearchcore.h"
#include "musicformats.h"

#include <QDir>
#if defined (Q_OS_ANDROID)
#   define QML_FILE_PREFIX  "file://"
#else
#   define QML_FILE_PREFIX  "file:///"
#endif

TTKFileSearchCore::TTKFileSearchCore(QObject *parent)
    : QThread(parent)
{
    m_run = true;
    m_type = Null_Type;
}

TTKFileSearchCore::~TTKFileSearchCore()
{
    stopAndQuitThread();
}

void TTKFileSearchCore::search(const QStringList &path)
{
    m_type = Search_Type;
    m_path.clear();

    foreach(QString var, path)
    {
        m_path << var.remove(QML_FILE_PREFIX);
    }

    start();
}

void TTKFileSearchCore::search(const QString &path)
{
    search(QStringList(path));
}

void TTKFileSearchCore::filesSize(const QString &dir)
{
    m_type = Size_Type;
    m_path.clear();
    m_path << dir;

    start();
}

void TTKFileSearchCore::start()
{
    m_run = true;
    QThread::start();
}

void TTKFileSearchCore::run()
{
    switch(m_type)
    {
        case Search_Type:
        {
            QFileInfoList list;
            foreach(const QString &path, m_path)
            {
                if(m_run)
                {
                    list << findFile(path, MusicFormats::supportFormatsFilterString());
                }
            }
            ///The name and path search ended when sending the corresponding

            QStringList pathes;
            foreach(const QFileInfo &var, list)
            {
                pathes << var.absoluteFilePath();
            }
            emit searchFinished(pathes);
            break;
        }
        case Size_Type:
        {
            quint64 size = 0;
            foreach(const QString &path, m_path)
            {
                if(m_run)
                {
                    size += dirSize(path);
                }
            }
            emit sizeFinished(size);
        }
        default: break;
    }
}

void TTKFileSearchCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

QFileInfoList TTKFileSearchCore::findFile(const QString &path, const QStringList &filter)
{
    QDir dir(path);
    if(!dir.exists() && !m_run)
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(const QFileInfo &files, fileList)
    {
        if(m_run)
        {
            emit findFilePath(QML_FILE_PREFIX + files.absoluteFilePath());
        }
    }

    foreach(const QFileInfo &folder, folderList)
    {
        if(m_run)
        {
            fileList.append( findFile(folder.absoluteFilePath(), filter) );
        }
    }
    return fileList;
}

quint64 TTKFileSearchCore::dirSize(const QString &dirName)
{
    quint64 size = 0;
    if(QFileInfo(dirName).isDir())
    {
        QDir dir(dirName);

        if(!m_run)
        {
            return size;
        }

        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs |  QDir::Hidden |
                                               QDir::NoSymLinks | QDir::NoDotAndDotDot);
        foreach(const QFileInfo &fileInfo, list)
        {
            if(!m_run)
            {
                return size;
            }

            if(fileInfo.isDir())
            {
                size += dirSize(fileInfo.absoluteFilePath());
            }
            else
            {
                size += fileInfo.size();
            }
        }
    }
    return size;
}
