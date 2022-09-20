#include "musicsongsmanagerthread.h"
#include "musicfileutils.h"
#include "musicformats.h"

MusicSongsManagerThread::MusicSongsManagerThread(QObject *parent)
    : MusicAbstractThread(parent)
{

}

void MusicSongsManagerThread::setFindFilePath(const QString &path)
{
    setFindFilePath(QStringList(path));
}

void MusicSongsManagerThread::setFindFilePath(const QStringList &path)
{
    m_path = path;
}

void MusicSongsManagerThread::run()
{
    MusicAbstractThread::run();

    QStringList list;
    for(const QString &path : qAsConst(m_path))
    {
        if(m_running)
        {
            list << MusicUtils::File::fileListByPath(path, MusicFormats::supportMusicInputFilterFormats());
        }
    }
    ///The name and path search ended when sending the corresponding
    Q_EMIT searchFilePathChanged(list);
}
