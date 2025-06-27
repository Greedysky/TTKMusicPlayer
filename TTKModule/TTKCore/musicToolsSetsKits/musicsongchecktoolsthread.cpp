#include "musicsongchecktoolsthread.h"
#include "musicsongmeta.h"

MusicSongCheckToolsRenameThread::MusicSongCheckToolsRenameThread(QObject *parent)
    : TTKAbstractThread(parent),
      m_items(nullptr),
      m_operateMode(TTK::Mode::Check)
{

}

void MusicSongCheckToolsRenameThread::setRenameSongs(MusicSongList *songs) noexcept
{
    m_items = songs;
}

void MusicSongCheckToolsRenameThread::run()
{
    if(m_items && !m_items->isEmpty())
    {
        if(m_operateMode == TTK::Mode::Check)
        {
            m_datas.clear();
            MusicSongMeta meta;
            for(const MusicSong &song : qAsConst(*m_items))
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

                if((!meta.artist().isEmpty() && !meta.title().isEmpty()) && (meta.artist() != song.artist() || meta.title() != song.title()))
                {
                    m_datas << MusicSongCheckToolsRename(song.name(), TTK::generateSongName(meta.title(), meta.artist()), song.path());
                }
            }
        }
        else
        {
            for(const int index : qAsConst(m_ids))
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
    m_items = nullptr;
}

void MusicSongCheckToolsDuplicateThread::setDuplicateSongs(MusicSongList *songs) noexcept
{
    m_items = songs;
}

void MusicSongCheckToolsDuplicateThread::run()
{
    if(m_items && !m_items->isEmpty())
    {
        if(m_operateMode == TTK::Mode::Check)
        {
            m_datas.clear();
            MusicSongMeta meta;
            for(const MusicSong &song : qAsConst(*m_items))
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
            for(const int index : qAsConst(m_ids))
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
    m_items = nullptr;
}

void MusicSongCheckToolsQualityThread::setQualitySongs(MusicSongList *songs) noexcept
{
    m_items = songs;
}

void MusicSongCheckToolsQualityThread::run()
{
    MusicSongCheckToolsQualityList items;
    if(m_items && !m_items->isEmpty())
    {
        MusicSongMeta meta;
        for(const MusicSong &song : qAsConst(*m_items))
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
