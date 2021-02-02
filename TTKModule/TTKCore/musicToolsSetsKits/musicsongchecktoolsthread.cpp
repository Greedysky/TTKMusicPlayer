#include "musicsongchecktoolsthread.h"
#include "musicsongmeta.h"

MusicSongCheckToolsRenameThread::MusicSongCheckToolsRenameThread(QObject *parent)
    : MusicAbstractThread(parent)
{
    m_songItems = nullptr;
    m_operateMode = MusicObject::Check;
}

void MusicSongCheckToolsRenameThread::setRenameSongs(MusicSongs *songs)
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
                    Q_EMIT finished(MusicSongCheckToolsRenames());
                    return;
                }

                if(!meta.read(song.getMusicPath()))
                {
                    continue;
                }

                if((!meta.getArtist().isEmpty() && !meta.getTitle().isEmpty()) &&
                    (meta.getArtist() != song.getMusicArtistFront() ||
                     meta.getTitle() != song.getMusicArtistBack()))
                {
                    m_datas << MusicSongCheckToolsRename(song.getMusicName(), meta.getArtist() + " - " + meta.getTitle(), song.getMusicPath());
                }
            }
        }
        else
        {
            for(const int index : qAsConst(m_itemIDs))
            {
                if(!m_running)
                {
                    Q_EMIT finished(MusicSongCheckToolsRenames());
                    return;
                }

                const MusicSongCheckToolsRename &song = m_datas[index];
                const QFileInfo info(song.m_filePath);
                QFile::rename(song.m_filePath, QString("%1%2%3.%4").arg(info.absolutePath()).arg("/").arg(song.m_RecommendName).arg(info.suffix()));
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

void MusicSongCheckToolsDuplicateThread::setDuplicateSongs(MusicSongs *songs)
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
                    Q_EMIT finished(MusicSongCheckToolsDuplicates());
                    return;
                }

                if(!meta.read(song.getMusicPath()))
                {
                    continue;
                }

                m_datas << MusicSongCheckToolsDuplicate(song, meta.getBitrate());
            }
        }
        else
        {
            for(const int index : qAsConst(m_itemIDs))
            {
                if(!m_run)
                {
                    Q_EMIT finished(MusicSongCheckToolsDuplicates());
                    return;
                }

                const MusicSongCheckToolsDuplicate &song = m_datas[index];
                QFile::remove(song.m_song.getMusicPath());
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

void MusicSongCheckToolsQualityThread::setQualitySongs(MusicSongs *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsQualityThread::run()
{
    MusicAbstractThread::run();

    MusicSongCheckToolsQualitys items;
    if(m_songItems && !m_songItems->isEmpty())
    {
        MusicSongMeta meta;
        for(const MusicSong &song : qAsConst(*m_songItems))
        {
            if(!m_running)
            {
                Q_EMIT finished(MusicSongCheckToolsQualitys());
                return;
            }

            if(!meta.read(song.getMusicPath()))
            {
                continue;
            }

            items << MusicSongCheckToolsQuality(song, meta.getBitrate());
        }
    }
    Q_EMIT finished(items);
}
