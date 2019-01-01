#include "musicsongchecktoolscore.h"
#include "musicsongtag.h"

MusicSongCheckToolsRenameCore::MusicSongCheckToolsRenameCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
    m_operateMode = MusicObject::Check;
}

MusicSongCheckToolsRenameCore::~MusicSongCheckToolsRenameCore()
{
    stopAndQuitThread();
}

void MusicSongCheckToolsRenameCore::setRenameSongs(MusicSongs *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsRenameCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsRenameCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsRenameCore::run()
{
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
                    emit finished(MusicSongCheckToolsRenames());
                    return;
                }

                if(!tag.read(song.getMusicPath()))
                {
                    continue;
                }

                if((!tag.getArtist().isEmpty() && !tag.getTitle().isEmpty()) &&
                    (tag.getArtist() != song.getMusicArtistFront() ||
                     tag.getTitle() != song.getMusicArtistBack()) )
                {
                    m_datas << MusicSongCheckToolsRename(song.getMusicName(), tag.getArtist() + " - " + tag.getTitle(), song.getMusicPath());
                }
            }
        }
        else
        {
            foreach(const int index, m_itemIDs)
            {
                if(!m_run)
                {
                    emit finished(MusicSongCheckToolsRenames());
                    return;
                }

                const MusicSongCheckToolsRename &song = m_datas[index];
                const QFileInfo info(song.m_filePath);
                QFile::rename(song.m_filePath, QString("%1%2%3.%4").arg(info.absolutePath()).arg("/").arg(song.m_RecommendName).arg(info.suffix()));
            }
        }
    }
    emit finished(m_datas);
}



MusicSongCheckToolsDuplicateCore::MusicSongCheckToolsDuplicateCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsDuplicateCore::~MusicSongCheckToolsDuplicateCore()
{
    stopAndQuitThread();
}

void MusicSongCheckToolsDuplicateCore::setDuplicateSongs(MusicSongs *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsDuplicateCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsDuplicateCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsDuplicateCore::run()
{
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
                    emit finished(MusicSongCheckToolsDuplicates());
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
                    emit finished(MusicSongCheckToolsDuplicates());
                    return;
                }

                const MusicSongCheckToolsDuplicate &song = m_datas[index];
                QFile::remove(song.m_song.getMusicPath());
            }
        }
    }
    emit finished(m_datas);
}



MusicSongCheckToolsQualityCore::MusicSongCheckToolsQualityCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsQualityCore::~MusicSongCheckToolsQualityCore()
{
    stopAndQuitThread();
}

void MusicSongCheckToolsQualityCore::setQualitySongs(MusicSongs *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsQualityCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsQualityCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsQualityCore::run()
{
    MusicSongCheckToolsQualitys items;
    if(m_songItems && !m_songItems->isEmpty())
    {
        MusicSongTag tag;
        foreach(const MusicSong &song, *m_songItems)
        {
            if(!m_run)
            {
                emit finished(MusicSongCheckToolsQualitys());
                return;
            }

            if(!tag.read(song.getMusicPath()))
            {
                continue;
            }

            items << MusicSongCheckToolsQuality(song, tag.getBitrate());
        }
    }
    emit finished(items);
}
