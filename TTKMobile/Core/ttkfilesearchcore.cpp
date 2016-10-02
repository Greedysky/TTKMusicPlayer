#include "ttkfilesearchcore.h"
#include "ttkmusicplayer.h"

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
}

TTKFileSearchCore::~TTKFileSearchCore()
{
    stopAndQuitThread();
}

void TTKFileSearchCore::search(const QStringList &path)
{
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

void TTKFileSearchCore::start()
{
    m_run = true;
    QThread::start();
}

void TTKFileSearchCore::run()
{
    QFileInfoList list;
    foreach(const QString &path, m_path)
    {
        if(m_run)
        {
            list << findFile(path, TTKMusicPlayer::supportFormatsFilterString());
        }
    }
    ///The name and path search ended when sending the corresponding

    QStringList pathes;
    foreach(const QFileInfo &var, list)
    {
        pathes << var.absoluteFilePath();
    }
    emit finished(pathes);
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
