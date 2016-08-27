#include "musicdownloadstatuslabel.h"
#include "musicapplication.h"
#include "musicbackgrounddownload.h"
#include "musicnetworkthread.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicnetworkthread.h"
#include "musicsongsearchonlinewidget.h"
#include "musicdownloadqueryfactory.h"

MusicDownloadStatusLabel::MusicDownloadStatusLabel(QWidget *w)
    : QObject(w)
{
    m_parentWidget = MStatic_cast(MusicApplication*, w);
    m_downloadLrcThread = nullptr;

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(MusicSongSearchOnlineTableWidget::getClassName(), getClassName());
    M_CONNECTION_PTR->poolConnect(MusicNetworkThread::getClassName(), getClassName());
}

MusicDownloadStatusLabel::~MusicDownloadStatusLabel()
{
    delete m_downloadLrcThread;
}

QString MusicDownloadStatusLabel::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownloadStatusLabel::showDownLoadInfoFor(MusicObject::DownLoadType type)
{
    QString stringType;
    switch(type)
    {
        case MusicObject::DW_DisConnection:
            stringType = "disconnection";
            break;
        case MusicObject::DW_DownLoading:
            stringType = "downloading";
            break;
        case MusicObject::DW_Buffing:
            stringType = "buffing";
            break;
        case MusicObject::DW_Waiting:
            break;
            stringType = "waiting";
        case MusicObject::DW_Null:
            break;
        default:
            break;
    }
    M_LOGGER_INFO(stringType);
}

void MusicDownloadStatusLabel::showDownLoadInfoFinished(const QString &type)
{
    ///If the lyrics download finished immediately loaded to display
    if(type == "Lrc")
    {
        m_parentWidget->musicLoadCurrentSongLrc();
    }
    m_parentWidget->updateCurrentArtist();
}

void MusicDownloadStatusLabel::networkConnectionStateChanged(bool state)
{
    showDownLoadInfoFor(state ? MusicObject::DW_Null : MusicObject::DW_DisConnection);
}

bool MusicDownloadStatusLabel::checkSettingParameterValue() const
{
    return ( M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool() ||
             M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool() );
}

void MusicDownloadStatusLabel::musicCheckHasLrcAlready()
{
    if(!M_NETWORK_PTR->isOnline())
    {
        ///no network connection
        showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    else if( checkSettingParameterValue() )
    {
        ///Check there is no opening lyrics display mode
       if( m_parentWidget->checkMusicListCurrentIndex() )
       {
           return;
       }

       QString filename = m_parentWidget->getCurrentFileName();
       ///Check if the file exists
       if( QFile::exists(MusicUtils::UCore::lrcPrefix() + filename + LRC_FILE) ||
           QFile::exists(MusicUtils::UCore::lrcPrefix() + filename + KRC_FILE) )
       {
           return;
       }

       if(m_downloadLrcThread)
       {
           delete m_downloadLrcThread;
           m_downloadLrcThread = nullptr;
       }
       ///Start the request query
       m_downloadLrcThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
       m_downloadLrcThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, filename);
       connect(m_downloadLrcThread, SIGNAL(downLoadDataChanged(QString)), SLOT(musicHaveNoLrcAlready()));
       showDownLoadInfoFor(MusicObject::DW_Buffing);
    }
}

void MusicDownloadStatusLabel::musicHaveNoLrcAlready()
{
    if(!M_NETWORK_PTR->isOnline())
    {   //no network connection
        showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }

    MusicObject::MusicSongInfomations musicSongInfos(m_downloadLrcThread->getMusicSongInfos());
    if(!musicSongInfos.isEmpty())
    {
        QString filename = m_parentWidget->getCurrentFileName();
        int count = filename.split('-').count();
        QString artistName = filename.split('-').front().trimmed();
        QString songName = filename.split('-').back().trimmed();

        MusicObject::MusicSongInfomation musicSongInfo = musicSongInfos.first();
        foreach(const MusicObject::MusicSongInfomation &var, musicSongInfos)
        {
            if( var.m_singerName.contains(artistName, Qt::CaseInsensitive) &&
                var.m_songName.contains(songName, Qt::CaseInsensitive) )
            {
                musicSongInfo = var;
                break;
            }
        }

        ///download lrc
        M_DOWNLOAD_QUERY_PTR->getDownloadLrc(musicSongInfo.m_lrcUrl, MusicUtils::UCore::lrcPrefix() + filename + LRC_FILE,
                                             MusicDownLoadThreadAbstract::Download_Lrc, this)->startToDownload();
        ///download art picture
        M_DOWNLOAD_QUERY_PTR->getDownloadSmallPic(musicSongInfo.m_smallPicUrl, ART_DIR_FULL + artistName + SKN_FILE,
                                                  MusicDownLoadThreadAbstract::Download_SmlBG, this)->startToDownload();
        ///download big picture
        (new MusicBackgroundDownload( count == 1 ? musicSongInfo.m_singerName : artistName, artistName, this))->startToDownload();
    }
    else
    {
        showDownLoadInfoFinished("find error!");
    }
}
