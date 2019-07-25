#include "musiclocalsongsmanagerthread.h"
#include "musicformats.h"
#include "musicfileutils.h"

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
    foreach(const QString &path, m_path)
    {
        if(m_run)
        {
            list << MusicUtils::File::getFileListByDir(path, MusicFormats::supportFormatsFilterString(), true);
        }
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


void MusicLocalSongsManagerThread::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}
