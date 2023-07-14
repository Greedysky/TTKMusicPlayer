#include "musicsongchecktoolsthread.h"
#include "musicsongmeta.h"

MusicSongCheckToolsRenameThread::MusicSongCheckToolsRenameThread(QObject *parent)
    : TTKAbstractThread(parent),
      m_songItems(nullptr),
      m_operateMode(TTK::Mode::Check)
{

}

void MusicSongCheckToolsRenameThread::setRenameSongs(MusicSongList *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsRenameThread::run()
{
    if(m_songItems && !m_songItems->isEmpty())
    {
        if(m_operateMode == TTK::Mode::Check)
        {
            m_datas.clear();
            MusicSongMeta meta;
            for(const MusicSong &song : qAsConst(*m_songItems))
            {
                if(!m_running)
                {
                    Q_EMIT finished(MusicSongCheckToolsRenameList());
                    return;
                }

                if(!meta.read(song.path()))
                {
                    continue;
                }

                if((!meta.artist().isEmpty() && !meta.title().isEmpty()) && (meta.artist() != song.artistFront() || meta.title() != song.artistBack()))
                {
                    m_datas << MusicSongCheckToolsRename(song.name(), meta.artist() + " - " + meta.title(), song.path());
                }
            }
        }
        else
        {
            for(const int index : qAsConst(m_itemIDs))
            {
                if(!m_running)
                {
                    Q_EMIT finished(MusicSongCheckToolsRenameList());
                    return;
                }

                const MusicSongCheckToolsRename &song = m_datas[index];
                const QFileInfo fin(song.m_path);
                QFile::rename(song.m_path, QString("%1/%2.%3").arg(fin.absolutePath(), song.m_recommendName, TTK_FILE_SUFFIX(fin)));
            }
        }
    }
    Q_EMIT finished(m_datas);
}



MusicSongCheckToolsDuplicateThread::MusicSongCheckToolsDuplicateThread(QObject *parent)
    : TTKAbstractThread(parent)
{
    m_songItems = nullptr;
}

void MusicSongCheckToolsDuplicateThread::setDuplicateSongs(MusicSongList *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsDuplicateThread::run()
{
    if(m_songItems && !m_songItems->isEmpty())
    {
        if(m_operateMode == TTK::Mode::Check)
        {
            m_datas.clear();
            MusicSongMeta meta;
            for(const MusicSong &song : qAsConst(*m_songItems))
            {
                if(!m_running)
                {
                    Q_EMIT finished(MusicSongCheckToolsDuplicateList());
                    return;
                }

                if(!meta.read(song.path()))
                {
                    continue;
                }

                m_datas << MusicSongCheckToolsDuplicate(song, meta.bitrate());
            }
        }
        else
        {
            for(const int index : qAsConst(m_itemIDs))
            {
                if(!m_running)
                {
                    Q_EMIT finished(MusicSongCheckToolsDuplicateList());
                    return;
                }

                const MusicSongCheckToolsDuplicate &song = m_datas[index];
                QFile::remove(song.m_song.path());
            }
        }
    }
    Q_EMIT finished(m_datas);
}



MusicSongCheckToolsQualityThread::MusicSongCheckToolsQualityThread(QObject *parent)
    : TTKAbstractThread(parent)
{
    m_songItems = nullptr;
}

void MusicSongCheckToolsQualityThread::setQualitySongs(MusicSongList *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsQualityThread::run()
{
    MusicSongCheckToolsQualityList items;
    if(m_songItems && !m_songItems->isEmpty())
    {
        MusicSongMeta meta;
        for(const MusicSong &song : qAsConst(*m_songItems))
        {
            if(!m_running)
            {
                Q_EMIT finished(MusicSongCheckToolsQualityList());
                return;
            }

            if(!meta.read(song.path()))
            {
                continue;
            }

            items << MusicSongCheckToolsQuality(song, meta.bitrate());
        }
    }
    Q_EMIT finished(items);
}
