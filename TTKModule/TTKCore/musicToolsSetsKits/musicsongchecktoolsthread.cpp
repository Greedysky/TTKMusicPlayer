#include "musicsongchecktoolsthread.h"
#include "musicsongtag.h"

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
            MusicSongTag tag;
            foreach(const MusicSong &song, *m_songItems)
            {
                if(!m_running)
                {
                    Q_EMIT finished(MusicSongCheckToolsRenames());
                    return;
                }

                if(!tag.read(song.getMusicPath()))
                {
                    continue;
                }

                if((!tag.getArtist().isEmpty() && !tag.getTitle().isEmpty()) &&
                    (tag.getArtist() != song.getMusicArtistFront() ||
                     tag.getTitle() != song.getMusicArtistBack()))
                {
                    m_datas << MusicSongCheckToolsRename(song.getMusicName(), tag.getArtist() + " - " + tag.getTitle(), song.getMusicPath());
                }
            }
        }
        else
        {
            foreach(const int index, m_itemIDs)
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
            MusicSongTag tag;
            foreach(const MusicSong &song, *m_songItems)
            {
                if(!m_run)
                {
                    Q_EMIT finished(MusicSongCheckToolsDuplicates());
                    return;
                }

                if(!tag.read(song.getMusicPath()))
                {
                    continue;
                }

                m_datas << MusicSongCheckToolsDuplicate(song, tag.getBitrate());
            }
        }
        else
        {
            foreach(const int index, m_itemIDs)
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
        MusicSongTag tag;
        foreach(const MusicSong &song, *m_songItems)
        {
            if(!m_running)
            {
                Q_EMIT finished(MusicSongCheckToolsQualitys());
                return;
            }

            if(!tag.read(song.getMusicPath()))
            {
                continue;
            }

            items << MusicSongCheckToolsQuality(song, tag.getBitrate());
        }
    }
    Q_EMIT finished(items);
}
