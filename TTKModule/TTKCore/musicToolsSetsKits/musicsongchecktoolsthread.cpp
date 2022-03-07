#include "musicsongchecktoolsthread.h"
#include "musicsongmeta.h"

MusicSongCheckToolsRenameThread::MusicSongCheckToolsRenameThread(QObject *parent)
    : MusicAbstractThread(parent)
{
    m_songItems = nullptr;
    m_operateMode = MusicObject::Check;
}

void MusicSongCheckToolsRenameThread::setRenameSongs(MusicSongList *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsRenameThread::run()
{
    MusicAbstractThread::run();

    if(m_songItems && !m_songItems->isEmpty())
    {
        if(m_operateMode == MusicObject::Check)
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

                if(!meta.read(song.musicPath()))
                {
                    continue;
                }

                if((!meta.artist().isEmpty() && !meta.title().isEmpty()) &&
                    (meta.artist() != song.musicArtistFront() ||
                     meta.title() != song.musicArtistBack()))
                {
                    m_datas << MusicSongCheckToolsRename(song.musicName(), meta.artist() + " - " + meta.title(), song.musicPath());
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
                QFile::rename(song.m_path, QString("%1/%2.%3").arg(fin.absolutePath(), song.m_recommendName, FILE_SUFFIX(fin)));
            }
        }
    }
    Q_EMIT finished(m_datas);
}



MusicSongCheckToolsDuplicateThread::MusicSongCheckToolsDuplicateThread(QObject *parent)
    : MusicAbstractThread(parent)
{
    m_songItems = nullptr;
}

void MusicSongCheckToolsDuplicateThread::setDuplicateSongs(MusicSongList *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsDuplicateThread::run()
{
    MusicAbstractThread::run();

    if(m_songItems && !m_songItems->isEmpty())
    {
        if(m_operateMode == MusicObject::Check)
        {
            m_datas.clear();
            MusicSongMeta meta;
            for(const MusicSong &song : qAsConst(*m_songItems))
            {
                if(!m_run)
                {
                    Q_EMIT finished(MusicSongCheckToolsDuplicateList());
                    return;
                }

                if(!meta.read(song.musicPath()))
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
                if(!m_run)
                {
                    Q_EMIT finished(MusicSongCheckToolsDuplicateList());
                    return;
                }

                const MusicSongCheckToolsDuplicate &song = m_datas[index];
                QFile::remove(song.m_song.musicPath());
            }
        }
    }
    Q_EMIT finished(m_datas);
}



MusicSongCheckToolsQualityThread::MusicSongCheckToolsQualityThread(QObject *parent)
    : MusicAbstractThread(parent)
{
    m_songItems = nullptr;
}

void MusicSongCheckToolsQualityThread::setQualitySongs(MusicSongList *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsQualityThread::run()
{
    MusicAbstractThread::run();

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

            if(!meta.read(song.musicPath()))
            {
                continue;
            }

            items << MusicSongCheckToolsQuality(song, meta.bitrate());
        }
    }
    Q_EMIT finished(items);
}
