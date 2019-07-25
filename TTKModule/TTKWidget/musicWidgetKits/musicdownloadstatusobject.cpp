#include "musicdownloadstatusobject.h"
#include "musicapplication.h"
#ifndef MUSIC_MOBILE
#include "musicbottomareawidget.h"
#include "musicsongsearchonlinewidget.h"
#endif
#include "musicnetworkthread.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicnetworkthread.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadbackgroundthread.h"
#include "musiccoreutils.h"
#include "musicstringutils.h"

MusicDownloadStatusObject::MusicDownloadStatusObject(QObject *parent)
    : QObject(parent)
{
    m_previousState = true;
    m_parentWidget = MStatic_cast(MusicApplication*, parent);
    m_downloadLrcThread = nullptr;

    M_CONNECTION_PTR->setValue(getClassName(), this);
#ifndef MUSIC_MOBILE
    M_CONNECTION_PTR->poolConnect(MusicSongSearchTableWidget::getClassName(), getClassName());
#endif
    M_CONNECTION_PTR->poolConnect(MusicNetworkThread::getClassName(), getClassName());
}

MusicDownloadStatusObject::~MusicDownloadStatusObject()
{
    delete m_downloadLrcThread;
}

void MusicDownloadStatusObject::showDownLoadInfoFor(MusicObject::DownLoadMode type)
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

void MusicDownloadStatusObject::showDownLoadInfoFinished(const QString &type)
{
    ///If the lyrics download finished immediately loaded to display
    if(type == "DownloadLrc")
    {
        m_parentWidget->musicLoadCurrentSongLrc();
    }
    else if(type == "DownloadSmallBackground")
    {
        m_parentWidget->updateCurrentArtist();
    }
}

void MusicDownloadStatusObject::networkConnectionStateChanged(bool state)
{
    M_NETWORK_PTR->setNetWorkState(state);
    if(m_previousState != state)
    {
#ifndef MUSIC_MOBILE
        MusicBottomAreaWidget *w = MusicBottomAreaWidget::instance();
        m_previousState ? w->showMessage(tr("TTKMusicPlayer"), tr("The Internet Seems To Be A Problem, Let's Listen To The Local Music."))
                        : w->showMessage(tr("TTKMusicPlayer"), tr("Network Connection Has Been Restored."));
#endif
    }
    m_previousState = state;
    showDownLoadInfoFor(state ? MusicObject::DW_Null : MusicObject::DW_DisConnection);
}

bool MusicDownloadStatusObject::checkSettingParameterValue() const
{
    return M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool() || M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool();
}

void MusicDownloadStatusObject::musicCheckHasLrcAlready()
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    else if(checkSettingParameterValue())
    {
        ///Check there is no opening lyrics display mode
       if(m_parentWidget->checkMusicListCurrentIndex())
       {
           return;
       }

       const QString &filename = m_parentWidget->getCurrentFileName();
       ///Check if the file exists
       if(QFile::exists(MusicUtils::String::lrcPrefix() + filename + LRC_FILE) || QFile::exists(MusicUtils::String::lrcPrefix() + filename + KRC_FILE))
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
       m_downloadLrcThread->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, filename);
       connect(m_downloadLrcThread, SIGNAL(downLoadDataChanged(QString)), SLOT(musicHaveNoLrcAlready()));
       showDownLoadInfoFor(MusicObject::DW_Buffing);
    }
}

void MusicDownloadStatusObject::musicHaveNoLrcAlready()
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(m_downloadLrcThread->getMusicSongInfos());
    if(!musicSongInfos.isEmpty())
    {
        const QString &filename = m_parentWidget->getCurrentFileName();
        const int count = MusicUtils::String::splitString(filename).count();
        const QString &artistName = MusicUtils::String::artistName(filename);
        const QString &songName = MusicUtils::String::songName(filename);

        MusicObject::MusicSongInformation musicSongInfo = musicSongInfos.first();
        foreach(const MusicObject::MusicSongInformation &var, musicSongInfos)
        {
            if(var.m_singerName.contains(artistName, Qt::CaseInsensitive) && var.m_songName.contains(songName, Qt::CaseInsensitive))
            {
                musicSongInfo = var;
                break;
            }
        }

        ///download lrc
        M_DOWNLOAD_QUERY_PTR->getDownloadLrcThread(musicSongInfo.m_lrcUrl, MusicUtils::String::lrcPrefix() + filename + LRC_FILE, MusicObject::DownloadLrc, this)->startToDownload();
        ///download art picture
        M_DOWNLOAD_QUERY_PTR->getDownloadSmallPicThread(musicSongInfo.m_smallPicUrl, ART_DIR_FULL + artistName + SKN_FILE, MusicObject::DownloadSmallBackground, this)->startToDownload();
        ///download big picture
        M_DOWNLOAD_QUERY_PTR->getDownloadBigPicThread( count == 1 ? musicSongInfo.m_singerName : artistName, artistName, this)->startToDownload();
    }
    else
    {
        showDownLoadInfoFinished("find error!");
    }
}
