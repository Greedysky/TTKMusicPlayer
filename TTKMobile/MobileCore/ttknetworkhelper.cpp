#include "ttknetworkhelper.h"
#include "musicdownloadquerymultiplethread.h"
#include "musiccryptographichash.h"
#include "musicdatadownloadthread.h"
#include "musictextdownloadthread.h"
#include "musicnetworkthread.h"
#include "musiccoreutils.h"
#///QJson import
#include "qjson/serializer.h"

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

void TTKNetworkHelper::setQueryType(int type)
{
    m_queryType = MStatic_cast(Type, type);
}

void TTKNetworkHelper::searchSong(const QString &text)
{
    m_queryType = T_SearcSong;
    m_queryThread->setQueryAllRecords(true);
    m_queryThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void TTKNetworkHelper::searchMovie(const QString &text)
{
    m_queryType = T_SearcMovie;
    m_queryThread->setQueryAllRecords(true);
    m_queryThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MovieQuery, text);
}

void TTKNetworkHelper::searchLrc(const QString &text)
{
    m_queryType = T_SearcLrc;
    m_queryThread->startSearchSong(MusicDownLoadQueryThreadAbstract::LrcQuery, text);
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
        case T_SearcMovie:
                    {
                        downForSearchMovie(index);
                        break;
                    }
        case T_SearcLrc:
                    {
                        downForSearchLrc(index);
                        break;
                    }
        case T_DownloadSong:
                    {
                        downForDownloadSong(data.toInt());
                        break;
                    }
        case T_DownloadSongIndex:
                    {
                        downForDownloadSong(index, data.toInt());
                        break;
                    }
        case T_DownloadMVIndex:
                    {
                        downForDownloadMovie(index, data.toInt());
                        break;
                    }
        default: break;
    }
}

QString TTKNetworkHelper::getSearchedAttributes(int index)
{
    MusicObject::MusicSongInfomations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(index < 0 || index >= musicSongInfos.count())
    {
        return QString();
    }

    QList<QVariant> jsonList;
    foreach(const MusicObject::MusicSongAttribute &attr, musicSongInfos[index].m_songAttrs)
    {
        QVariantMap jsonMap;
        jsonMap["bitrate"] = attr.m_bitrate;
        jsonMap["format"] = attr.m_format;
        jsonMap["url"] = attr.m_url;
        jsonMap["size"] = attr.m_size;

        jsonList << jsonMap;
    }

    QJson::Serializer serializer;
    bool ok;
    QByteArray data(serializer.serialize(jsonList, &ok));
    return ok ? QString(data) : QString();
}

void TTKNetworkHelper::downLoadDataChanged()
{
    emit downLoadDataHasFinished( m_queryThread->getMusicSongInfos().isEmpty() );
    switch(m_queryType)
    {
        case T_SearcSong: break;
        case T_SearcMovie: break;
        case T_SearcLrc: break;
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
    emit downForSearchSongFinished( musicEnSong, downloadName );
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
    foreach(const MusicObject::MusicSongAttribute &attr, musicSongInfo.m_songAttrs)
    {
        if(musicSongAttr.m_bitrate > attr.m_bitrate)
        {
            musicSongAttr = attr;
        }
    }

    QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
    QString musicEnSong = MusicCryptographicHash().encrypt(musicSong, DOWNLOAD_KEY);
    QString downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(musicSongAttr.m_format);
    MusicDataDownloadThread *downSong = new MusicDataDownloadThread( musicSongAttr.m_url, downloadName,
                                                                     MusicDownLoadThreadAbstract::Download_Music, this);
    connect(downSong, SIGNAL(downLoadDataChanged(QString)), SLOT(searchDataDwonloadFinished()));
    downSong->startToDownload();
    m_currentIndex = index;
}

void TTKNetworkHelper::downForSearchMovie(int index)
{
    m_currentIndex = -1;
    MusicObject::MusicSongInfomations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(index < 0 || index >= musicSongInfos.count())
    {
        return;
    }

    MusicObject::MusicSongInfomation musicSongInfo = musicSongInfos[index];
    foreach(const MusicObject::MusicSongAttribute &musicAttr, musicSongInfo.m_songAttrs)
    {
        emit downForSearchMovieFinished(musicAttr.m_url);
        return;
    }
    emit downForSearchMovieFinished( QString() );
}

void TTKNetworkHelper::downForSearchLrc(int index)
{
    m_currentIndex = -1;
    MusicObject::MusicSongInfomations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(index < 0 || index >= musicSongInfos.count())
    {
        return;
    }

    MusicObject::MusicSongInfomation musicSongInfo = musicSongInfos[index];
    QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
    musicSong = QString("%1%2.%3").arg(MusicUtils::Core::lrcPrefix()).arg(musicSong).arg(LRC_FILE);

    MusicTextDownLoadThread *downSong = new MusicTextDownLoadThread(musicSongInfo.m_lrcUrl, musicSong,
                                            MusicDownLoadThreadAbstract::Download_Lrc, this);
    QEventLoop loop(this);
    connect(downSong, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    downSong->startToDownload();
    loop.exec();

    emit downForSearchLrcFinished(musicSong);
}

void TTKNetworkHelper::downForDownloadSong(int bitrate)
{
    downForDownloadSong(0, bitrate);
}

void TTKNetworkHelper::downForDownloadSong(int index, int bitrate)
{
    m_currentIndex = -1;
    MusicObject::MusicSongInfomations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(index < 0 || index >= musicSongInfos.count())
    {
        return;
    }

    MusicObject::MusicSongInfomation musicSongInfo(musicSongInfos[index]);
    MusicObject::MusicSongAttributes musicAttrs = musicSongInfo.m_songAttrs;
    foreach(const MusicObject::MusicSongAttribute &musicAttr, musicAttrs)
    {
        if(bitrate == musicAttr.m_bitrate)
        {
            QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
            musicSong = QString("%1%2.%3").arg(MusicUtils::Core::musicPrefix()).arg(musicSong).arg(musicAttr.m_format);
            if(!QFile::exists(musicSong))
            {
                MusicDataDownloadThread *downSong = new MusicDataDownloadThread( musicAttr.m_url, musicSong,
                                                        MusicDownLoadThreadAbstract::Download_Music, this);
                QEventLoop loop(this);
                connect(downSong, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
                downSong->startToDownload();
                loop.exec();
                emit downForDownloadSongFinished(musicSong);
            }
            return;
        }
    }
}

void TTKNetworkHelper::downForDownloadMovie(int index, int bitrate)
{
    m_currentIndex = -1;
    MusicObject::MusicSongInfomations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(index < 0 || index >= musicSongInfos.count())
    {
        return;
    }

    MusicObject::MusicSongInfomation musicSongInfo(musicSongInfos[index]);
    MusicObject::MusicSongAttributes musicAttrs = musicSongInfo.m_songAttrs;
    foreach(const MusicObject::MusicSongAttribute &musicAttr, musicAttrs)
    {
        if(bitrate == musicAttr.m_bitrate)
        {
            QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
            musicSong = QString("%1%2.%3").arg(MOVIE_DIR_FULL).arg(musicSong).arg(musicAttr.m_format);
            if(!QFile::exists(musicSong))
            {
                MusicDataDownloadThread *downSong = new MusicDataDownloadThread( musicAttr.m_url, musicSong,
                                                        MusicDownLoadThreadAbstract::Download_Video, this);
                QEventLoop loop(this);
                connect(downSong, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
                downSong->startToDownload();
                loop.exec();
                emit downForDownloadMovieFinished(musicSong);
            }
            return;
        }
    }
}
