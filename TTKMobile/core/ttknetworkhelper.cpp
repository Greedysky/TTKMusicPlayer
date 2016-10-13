#include "ttknetworkhelper.h"
#include "musicdownloadquerymultiplethread.h"
#include "musiccryptographichash.h"
#include "musicdatadownloadthread.h"

TTKNetworkHelper::TTKNetworkHelper(QObject *parent)
    : QObject(parent)
{
    m_currentIndex = -1;
    m_queryThread = new MusicDownLoadQueryMultipleThread(this);
    connect(m_queryThread, SIGNAL(clearAllItems()), SIGNAL(clearAllItems()));
    connect(m_queryThread, SIGNAL(createSearchedItems(QString,QString,QString)),
                           SIGNAL(createSearchedItems(QString,QString)));
}

TTKNetworkHelper::~TTKNetworkHelper()
{
    delete m_queryThread;
}

void TTKNetworkHelper::searchSong(const QString &text)
{
    m_queryThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void TTKNetworkHelper::setCurrentIndex(int index)
{
    m_currentIndex = -1;
    MusicObject::MusicSongInfomations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(index < 0 || index >= musicSongInfos.count())
    {
        return;
    }

    MusicObject::MusicSongInfomation musicSongInfo = musicSongInfos[index];
    MusicObject::MusicSongAttribute musicSongAttr = musicSongInfo.m_songAttrs.first();
    QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
    QString musicEnSong = MusicCryptographicHash().encrypt(musicSong, DOWNLOAD_KEY);
    QString downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(musicSongAttr.m_format);
    MusicDataDownloadThread *downSong = new MusicDataDownloadThread( musicSongAttr.m_url, downloadName,
                                                                     MusicDownLoadThreadAbstract::Download_Music, this);
    connect(downSong, SIGNAL(downLoadDataChanged(QString)), SLOT(searchDataDwonloadFinished()));
    downSong->startToDownload();
    m_currentIndex = index;
}

void TTKNetworkHelper::searchDataDwonloadFinished()
{
    MusicObject::MusicSongInfomations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(m_currentIndex < 0 || m_currentIndex >= musicSongInfos.count())
    {
        return;
    }

    MusicObject::MusicSongInfomation musicSongInfo = musicSongInfos[m_currentIndex];
    MusicObject::MusicSongAttribute musicSongAttr = musicSongInfo.m_songAttrs.first();
    QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
    QString musicEnSong = MusicCryptographicHash().encrypt(musicSong, DOWNLOAD_KEY);
    QString downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(musicSongAttr.m_format);
    emit downloadFinished( musicEnSong, downloadName );
}
