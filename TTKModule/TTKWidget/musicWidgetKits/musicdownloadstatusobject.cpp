#include "musicdownloadstatusobject.h"
#include "musicapplication.h"
#ifndef MUSIC_MOBILE
#include "musicbottomareawidget.h"
#include "musicsongsearchonlinewidget.h"
#endif
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadbackgroundrequest.h"
#include "musiccoreutils.h"
#include "musicstringutils.h"

MusicDownloadStatusObject::MusicDownloadStatusObject(QObject *parent)
    : QObject(parent)
{
    m_previousState = true;
    m_parentWidget = TTKStatic_cast(MusicApplication*, parent);
    m_downloadRequest = nullptr;

    M_CONNECTION_PTR->setValue(getClassName(), this);
#ifndef MUSIC_MOBILE
    M_CONNECTION_PTR->poolConnect(MusicSongSearchTableWidget::getClassName(), getClassName());
#endif
    M_CONNECTION_PTR->poolConnect(MusicNetworkThread::getClassName(), getClassName());
}

MusicDownloadStatusObject::~MusicDownloadStatusObject()
{
    delete m_downloadRequest;
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
    return M_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toBool() || M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool();
}

void MusicDownloadStatusObject::checkLrcValid()
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

       if(m_downloadRequest)
       {
           delete m_downloadRequest;
           m_downloadRequest = nullptr;
       }
       ///Start the request query
       m_downloadRequest = M_DOWNLOAD_QUERY_PTR->getQueryRequest(this);
       m_downloadRequest->startToSearch(MusicAbstractQueryRequest::MusicQuery, filename);
       connect(m_downloadRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(currentLrcDataDownload()));
       showDownLoadInfoFor(MusicObject::DW_Buffing);
    }
}

void MusicDownloadStatusObject::currentLrcDataDownload()
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(m_downloadRequest->getMusicSongInfos());
    if(!musicSongInfos.isEmpty())
    {
        const QString &filename = m_parentWidget->getCurrentFileName();
        const int count = MusicUtils::String::stringSplit(filename).count();
        const QString &artistName = MusicUtils::String::artistName(filename);
        const QString &songName = MusicUtils::String::songName(filename);

        MusicObject::MusicSongInformation musicSongInfo = musicSongInfos.first();
        for(const MusicObject::MusicSongInformation &var : qAsConst(musicSongInfos))
        {
            if(var.m_singerName.contains(artistName, Qt::CaseInsensitive) && var.m_songName.contains(songName, Qt::CaseInsensitive))
            {
                musicSongInfo = var;
                break;
            }
        }

        ///download lrc
        M_DOWNLOAD_QUERY_PTR->getDownloadLrcRequest(musicSongInfo.m_lrcUrl, MusicUtils::String::lrcPrefix() + filename + LRC_FILE, MusicObject::DownloadLrc, this)->startToDownload();
        ///download art picture
        M_DOWNLOAD_QUERY_PTR->getDownloadSmallPictureRequest(musicSongInfo.m_smallPicUrl, ART_DIR_FULL + artistName + SKN_FILE, MusicObject::DownloadSmallBackground, this)->startToDownload();
        ///download big picture
        M_DOWNLOAD_QUERY_PTR->getDownloadBigPictureRequest(count == 1 ? musicSongInfo.m_singerName : artistName, artistName, this)->startToDownload();
    }
    else
    {
        showDownLoadInfoFinished("find error");
    }
}
