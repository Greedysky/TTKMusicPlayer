#include "ttknetworkhelper.h"
#include "musicdownloadqueryfactory.h"
#include "musicdatatagdownloadthread.h"
#include "musiccryptographichash.h"
#include "musicnetworkthread.h"
#include "musiccoreutils.h"
#include "musicsettingmanager.h"
#include "ttkmusicutils.h"
#///QJson import
#include "qjson/serializer.h"

TTKNetworkHelper::TTKNetworkHelper(QObject *parent)
    : QObject(parent)
{
    M_NETWORK_PTR->start();

    m_queryType = Null;
    m_queryThread = nullptr;

    connect(M_NETWORK_PTR, SIGNAL(networkConnectionStateChanged(bool)),
                           SIGNAL(networkConnectionStateChanged(bool)));
}

TTKNetworkHelper::~TTKNetworkHelper()
{
    closeWindowNotify();
    delete m_queryThread;
}

void TTKNetworkHelper::abort()
{
    if(m_queryThread)
    {
        m_queryThread->setNetworkAbort(true);
    }
}

void TTKNetworkHelper::setQueryType(int type)
{
    m_queryType = MStatic_cast(Type, type);
}

void TTKNetworkHelper::searchSong(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())
    {
        return;
    }

    m_queryType = SearcSong;
    m_queryThread->setQueryAllRecords(true);
    m_queryThread->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void TTKNetworkHelper::setCurrentIndex(int index, const QVariant &data)
{
    if(!M_NETWORK_PTR->isOnline())
    {
        return;
    }

    if(m_queryType == DownloadSongIndex)
    {
        downForDownloadSong(index, data.toInt());
    }
}

QString TTKNetworkHelper::getSearchedAttributes(int index)
{
    MusicObject::MusicSongInformations musicSongInfos(m_queryThread->getMusicSongInfos());
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

void TTKNetworkHelper::setCurrentServer()
{
    int index= getCurrentServer();
    setCurrentServer(index);
}

void TTKNetworkHelper::setCurrentServer(int index)
{
    delete m_queryThread;

    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerChoiced, index);

    m_queryThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_queryThread, SIGNAL(clearAllItems()), SIGNAL(clearAllItems()));
    connect(m_queryThread, SIGNAL(createSearchedItems(MusicSearchedItem)), SLOT(createSearchedItems(MusicSearchedItem)));
    connect(m_queryThread, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataChanged()));
}

int TTKNetworkHelper::getCurrentServer() const
{
    return M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
}

QString TTKNetworkHelper::getCurrentServerString() const
{
    return m_queryThread->mapQueryServerString();
}

void TTKNetworkHelper::downLoadDataChanged()
{
    emit queryDataHasFinished( m_queryThread->getMusicSongInfos().isEmpty() );
}

void TTKNetworkHelper::createSearchedItems(const MusicSearchedItem &songItem)
{
    emit createSearchedItems(songItem.m_songName, songItem.m_singerName);
}

void TTKNetworkHelper::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    Q_UNUSED(total);
    Q_UNUSED(time);
    if(percent < 100)
    {
        TTKMusicUtils().showWindowNotify(tr("Downloading"), tr("%1 percent").arg(MStatic_cast(int, percent)), percent);
    }
    else
    {
        closeWindowNotify();
    }
}

void TTKNetworkHelper::closeWindowNotify()
{
    TTKMusicUtils().showWindowNotify(100);
}

void TTKNetworkHelper::downForDownloadSong(int index, int bitrate)
{
    MusicObject::MusicSongInformations musicSongInfos(m_queryThread->getMusicSongInfos());
    if(index < 0 || index >= musicSongInfos.count() || !M_NETWORK_PTR->isOnline())
    {
        return;
    }

    MusicObject::MusicSongInformation musicSongInfo(musicSongInfos[index]);
    MusicObject::MusicSongAttributes musicAttrs = musicSongInfo.m_songAttrs;
    foreach(const MusicObject::MusicSongAttribute &musicAttr, musicAttrs)
    {
        if(bitrate == musicAttr.m_bitrate)
        {
            QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
            QString downloadPrefix = MusicUtils::Core::musicPrefix();
            QString downloadName = QString("%1%2.%3").arg(downloadPrefix).arg(musicSong).arg(musicAttr.m_format);

            QFile file(downloadName);
            if(file.exists())
            {
                for(int i=1; i<99; ++i)
                {
                    if(!QFile::exists(downloadName))
                    {
                        break;
                    }
                    if(i != 1)
                    {
                        musicSong.chop(3);
                    }
                    musicSong += QString("(%1)").arg(i);
                    downloadName = QString("%1%2.%3").arg(downloadPrefix).arg(musicSong).arg(musicAttr.m_format);
                }
            }
            ////////////////////////////////////////////////
            MusicDataTagDownloadThread *downSong = new MusicDataTagDownloadThread( musicAttr.m_url, downloadName,
                                                                                   MusicDownLoadThreadAbstract::Download_Music, this);
            connect(downSong, SIGNAL(downLoadDataChanged(QString)), SIGNAL(downLoadDataHasFinished()));
            downSong->setTags(musicSongInfo.m_smallPicUrl, musicSongInfo.m_songName, musicSongInfo.m_singerName, musicSongInfo.m_albumName);
            downSong->startToDownload();
            break;
        }
    }
}
