#include "ttknetworkhelper.h"
#include "musicdownloadquerymultiplethread.h"
#include "musiccryptographichash.h"
#include "musicdatadownloadthread.h"
#include "musicnetworkthread.h"

TTKNetworkHelper::TTKNetworkHelper(QObject *parent)
    : QObject(parent)
{
    m_currentIndex = -1;
    m_queryType = T_Null;
    m_queryThread = new MusicDownLoadQueryMultipleThread(this);
    connect(m_queryThread, SIGNAL(clearAllItems()), SIGNAL(clearAllItems()));
    connect(m_queryThread, SIGNAL(createSearchedItems(QString,QString,QString)),
                           SIGNAL(createSearchedItems(QString,QString)));
    connect(m_queryThread, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataChanged()));
}

TTKNetworkHelper::~TTKNetworkHelper()
{
    delete m_queryThread;
}

void TTKNetworkHelper::searchSong(const QString &text)
{
    m_queryType = T_SearcSong;
    m_queryThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void TTKNetworkHelper::downloadSong(const QString &text)
{
    m_queryType = T_DownloadSong;
    m_queryThread->setQueryAllRecords(true);
    m_queryThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void TTKNetworkHelper::setCurrentIndex(int index, const QVariant &data)
{
    switch(m_queryType)
    {
        case T_SearcSong:
                    {
                        downForSearchSong(index);
                        break;
                    }
        case T_DownloadSong:
                    {
                        downForDownloadSong(data.toInt());
                        break;
                    }
        default: break;
    }
}

void TTKNetworkHelper::downLoadDataChanged()
{
    switch(m_queryType)
    {
        case T_SearcSong: break;
        case T_DownloadSong:
                    {
                        dataForDownloadSong();
                        break;
                    }
        default: break;
    }
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

void TTKNetworkHelper::dataForDownloadSong()
{
    MusicObject::MusicSongInfomations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(!musicSongInfos.isEmpty())
    {
        MusicObject::MusicSongInfomation musicSongInfo(musicSongInfos.first());
        MusicObject::MusicSongAttributes musicAttrs = musicSongInfo.m_songAttrs;
        foreach(const MusicObject::MusicSongAttribute &musicAttr, musicAttrs)
        {
            emit createDownloadSongQuality(musicAttr.m_bitrate);
        }
    }
    else
    {
        emit createDownloadSongQuality(-1);
    }
}

void TTKNetworkHelper::downForSearchSong(int index)
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

void TTKNetworkHelper::downForDownloadSong(int bitrate)
{
    MusicObject::MusicSongInfomations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        return;
    }

    QString downloadUrl;
    MusicObject::MusicSongInfomation musicSongInfo(musicSongInfos.first());
    MusicObject::MusicSongAttributes musicAttrs = musicSongInfo.m_songAttrs;
    foreach(const MusicObject::MusicSongAttribute &musicAttr, musicAttrs)
    {
        if(bitrate == musicAttr.m_bitrate)
        {
            downloadUrl = musicAttr.m_url;
        }
    }

    qDebug() << downloadUrl;
//    MusicDataDownloadThread *downSong = new MusicDataDownloadThread( downloadUrl, downloadName,
//                                                                     MusicDownLoadThreadAbstract::Download_Music, this);
//    connect(downSong, SIGNAL(downLoadDataChanged(QString)), SLOT(searchDataDwonloadFinished()));
//    downSong->startToDownload();
}
