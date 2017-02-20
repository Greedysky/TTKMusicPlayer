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

QString MusicSongCheckToolsRenameCore::getClassName()
{
    return staticMetaObject.className();
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
                    emit finished(SongCheckToolsRenames());
                    return;
                }

                if(!tag.readFile(song.getMusicPath()))
                {
                    continue;
                }

                if((!tag.getArtist().isEmpty() && !tag.getTitle().isEmpty()) &&
                     (tag.getArtist() != song.getMusicArtistFront() ||
                      tag.getTitle() != song.getMusicArtistBack()) )
                {
                    m_datas << SongCheckToolsRename(song.getMusicName(), tag.getArtist() + " - " + tag.getTitle(),
                                                    song.getMusicPath());
                }
            }
        }
        else
        {
            foreach(const int index, m_itemIDs)
            {
                if(!m_run)
                {
                    emit finished(SongCheckToolsRenames());
                    return;
                }

                const SongCheckToolsRename song = m_datas[index];
                QFileInfo info(song.m_filePath);
                QFile::rename(song.m_filePath, QString("%1%2%3.%4").arg(info.absolutePath())
                                               .arg(QDir::separator()).arg(song.m_RecommendName)
                                               .arg(info.suffix()));
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

QString MusicSongCheckToolsDuplicateCore::getClassName()
{
    return staticMetaObject.className();
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
                    emit finished(SongCheckToolsDuplicates());
                    return;
                }

                if(!tag.readFile(song.getMusicPath()))
                {
                    continue;
                }

                m_datas << SongCheckToolsDuplicate(song, tag.getBitrate());
            }
        }
        else
        {
            foreach(const int index, m_itemIDs)
            {
                if(!m_run)
                {
                    emit finished(SongCheckToolsDuplicates());
                    return;
                }

                const SongCheckToolsDuplicate song = m_datas[index];
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

QString MusicSongCheckToolsQualityCore::getClassName()
{
    return staticMetaObject.className();
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
    SongCheckToolsQualitys items;
    if(m_songItems && !m_songItems->isEmpty())
    {
        MusicSongTag tag;
        foreach(const MusicSong &song, *m_songItems)
        {
            if(!m_run)
            {
                emit finished(SongCheckToolsQualitys());
                return;
            }

            if(!tag.readFile(song.getMusicPath()))
            {
                continue;
            }

            items << SongCheckToolsQuality(song, tag.getBitrate(), transfromBitrateToQuality(tag.getBitrate()));
        }
    }
    emit finished(items);
}

int MusicSongCheckToolsQualityCore::transfromBitrateToQuality(const QString &bitrate) const
{
    if(bitrate.isEmpty())
    {
        return -1;
    }

    QStringList data(bitrate.split(" "));
    if(data.count() >= 2)
    {
        int bit = data.front().trimmed().toInt();
        if(bit <= 0)
        {
            return -1;
        }
        else if(bit > 0 && bit <= 96)
        {
            return 0;
        }
        else if(bit > 96 && bit <= 196)
        {
            return 1;
        }
        else if(bit > 196 && bit <= 320)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }
    return -1;
}
