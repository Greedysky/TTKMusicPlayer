#include "musicdownloadstatuslabel.h"
#include "musicapplication.h"
#include "musictextdownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicdata2downloadthread.h"
#include "musicbackgrounddownload.h"
#include "musicnetworkthread.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicnetworkthread.h"
#include "musicsongsearchonlinewidget.h"

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
       if( QFile::exists(LRC_DIR_FULL + filename + LRC_FILE) ||
           QFile::exists(LRC_DIR_FULL + filename + KRC_FILE) )
       {
           return;
       }

       if(m_downloadLrcThread)
       {
           delete m_downloadLrcThread;
           m_downloadLrcThread = nullptr;
       }
       ///Start the request query
#ifndef USE_MULTIPLE_QUERY
       m_downloadLrcThread = new MusicDownLoadQuerySingleThread(this);
#else
       m_downloadLrcThread = new MusicDownLoadQueryMultipleThread(this);
#endif
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
        foreach(MusicObject::MusicSongInfomation var, musicSongInfos)
        {
            if( var.m_singerName.contains(artistName, Qt::CaseInsensitive) &&
                var.m_songName.contains(songName, Qt::CaseInsensitive) )
            {
                musicSongInfo = var;
                break;
            }
        }

        ///download lrc
        MusicTextDownLoadThread* lrc = new MusicTextDownLoadThread(musicSongInfo.m_lrcUrl,
                                 LRC_DIR_FULL + filename + LRC_FILE,
                                 MusicDownLoadThreadAbstract::Download_Lrc, this);
        lrc->startToDownload();

        ///download art picture
#ifndef USE_MULTIPLE_QUERY
        (new MusicData2DownloadThread(musicSongInfo.m_smallPicUrl, ART_DIR_FULL + artistName + SKN_FILE,
                                 MusicDownLoadThreadAbstract::Download_SmlBG, this))->startToDownload();
#else
        (new MusicDataDownloadThread(musicSongInfo.m_smallPicUrl, ART_DIR_FULL + artistName + SKN_FILE,
                                 MusicDownLoadThreadAbstract::Download_SmlBG, this))->startToDownload();
#endif
        ///download big picture
        (new MusicBackgroundDownload( count == 1 ? musicSongInfo.m_singerName : artistName, artistName, this))->startToDownload();
    }
    else
    {
        showDownLoadInfoFinished("find error!");
    }
}
