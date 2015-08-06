#include "musiclocalsongsmanagerthread.h"
#include <QDir>
#include <QDebug>

MusicLocalSongsManagerThread::MusicLocalSongsManagerThread(QObject *parent)
            : QThread(parent)
{
    m_run = true;
}

MusicLocalSongsManagerThread::~MusicLocalSongsManagerThread()
{
    stopAndQuitThread();
}

void MusicLocalSongsManagerThread::run()
{
    for(int i=0; i<m_path.count(); ++i)
        findFile(m_path[i]);

    ///The name and path search ended when sending the corresponding
    emit setSongNamePath(m_filename, m_fileDir);
}

void MusicLocalSongsManagerThread::start()
{
    m_run = true;
    QThread::start();
}

void MusicLocalSongsManagerThread::setFindFilePath(const QString &path)
{
    setFindFilePath(QStringList(path));
}

void MusicLocalSongsManagerThread::setFindFilePath(const QStringList &path)
{
    m_path = path;
    m_filename.clear();
    m_fileDir.clear();
}

void MusicLocalSongsManagerThread::findFile(const QString &path)
{
    ///Find the corresponding suffix name
    QDir dir(path);
    if(!m_run || !dir.exists())
        return;
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);

    QFileInfoList list = dir.entryInfoList();
    if(list.count() == 0) return;
    int i=0;
    do{
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName() == "." | fileInfo.fileName() == ".." )
        {
            ++i;
            continue;
        }
        if(fileInfo.isDir())
        {
            m_filename<<fileInfo.fileName();
            m_fileDir<<fileInfo.path();
            findFile(fileInfo.filePath());
        }
        else
        {
            m_filename<<fileInfo.fileName();
            m_fileDir<<fileInfo.path();
        }
        ++i;
    }while(i < list.size());
}

void MusicLocalSongsManagerThread::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}
