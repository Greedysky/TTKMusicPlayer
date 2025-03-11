#include "musicsongsmanagerthread.h"
#include "musicfileutils.h"
#include "musicformats.h"

MusicSongsManagerThread::MusicSongsManagerThread(QObject *parent)
    : TTKAbstractThread(parent)
{

}

void MusicSongsManagerThread::setFindFilePath(const QString &path) noexcept
{
    setFindFilePath(QStringList(path));
}

void MusicSongsManagerThread::setFindFilePath(const QStringList &path) noexcept
{
    m_path = path;
}

void MusicSongsManagerThread::run()
{
    QStringList list;
    for(const QString &path : qAsConst(m_path))
    {
        if(m_running)
        {
            list << TTK::File::fileListByPath(path, MusicFormats::supportMusicInputFilterFormats());
        }
    }
    ///The name and path search ended when sending the corresponding
    Q_EMIT searchFilePathChanged(list);
}
