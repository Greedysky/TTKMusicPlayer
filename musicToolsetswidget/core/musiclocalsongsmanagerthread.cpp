#include "musiclocalsongsmanagerthread.h"
#include <QDir>
#include "musicplayer.h"

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
    QFileInfoList list;
    for(int i=0; i<m_path.count(); ++i)
    {
        list << findFile(m_path[i]);
    }

    ///The name and path search ended when sending the corresponding
    emit setSongNamePath( list );
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
}

QFileInfoList MusicLocalSongsManagerThread::findFile(const QString &path)
{
    ///Find the corresponding suffix name
    QDir dir(path);
    if(!m_run || !dir.exists())
    {
        return QFileInfoList();
    }
    QFileInfoList fileList = dir.entryInfoList(MusicPlayer::supportFormatsFilterString(),
                                               QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i=0; i<folderList.size(); ++i)
    {
        QString childPath = folderList[i].absoluteFilePath();
        fileList.append( findFile(childPath) );
    }
    return fileList;
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
