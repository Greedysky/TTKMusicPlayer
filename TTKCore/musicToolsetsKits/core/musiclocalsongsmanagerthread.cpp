#include "musiclocalsongsmanagerthread.h"
#include "musicformats.h"
#include "musiccoreutils.h"

MusicLocalSongsManagerThread::MusicLocalSongsManagerThread(QObject *parent)
    : QThread(parent)
{
    m_run = true;
}

MusicLocalSongsManagerThread::~MusicLocalSongsManagerThread()
{
    stopAndQuitThread();
}

QString MusicLocalSongsManagerThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicLocalSongsManagerThread::run()
{
    QFileInfoList list;
    foreach(const QString &path, m_path)
    {
        if(m_run)
        {
            list << MusicUtils::Core::findFile(path, MusicFormats::supportFormatsFilterString());
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
