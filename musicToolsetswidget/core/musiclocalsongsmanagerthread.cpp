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
    foreach(QString path, m_path)
    {
        list << findFile(path);
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

    foreach(QFileInfo folder, folderList)
    {
        fileList.append( findFile(folder.absoluteFilePath()) );
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
