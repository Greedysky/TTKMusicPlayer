#include "musicdownloadstatusmodule.h"
#include "musicsongsearchonlinewidget.h"
#include "musicapplication.h"
#include "musicbottomareawidget.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadimagerequest.h"

MusicDownloadStatusModule::MusicDownloadStatusModule(QObject *parent)
    : QObject(parent),
      m_previousState(true),
      m_parentClass(TTKObject_cast(MusicApplication*, parent))
{
    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(MusicSongSearchTableWidget::className(), className());
    G_CONNECTION_PTR->poolConnect(MusicNetworkThread::className(), className());
}

void MusicDownloadStatusModule::showDownLoadInfoFinished(const QString &bytes)
{
    ///If the lyrics download finished immediately loaded to display
    if(bytes == DOWNLOAD_KEY_LRC)
    {
        m_parentClass->musicLoadCurrentSongLrc();
    }
    else if(bytes == DOWNLOAD_KEY_COVER)
    {
        m_parentClass->updateCurrentArtist();
    }
}

void MusicDownloadStatusModule::networkConnectionStateChanged(bool state)
{
    G_NETWORK_PTR->setNetWorkState(state);
    if(m_previousState != state)
    {
        MusicBottomAreaWidget *w = MusicBottomAreaWidget::instance();
        m_previousState ? w->showMessage(tr("TTKMusicPlayer"), tr("The Internet Seems To Be A Problem, Let's Listen To The Local Music."))
                        : w->showMessage(tr("TTKMusicPlayer"), tr("Network Connection Has Been Restored."));
    }
    m_previousState = state;
}

bool MusicDownloadStatusModule::checkSettingParameterValue() const
{
    return G_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toBool() || G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool();
}

void MusicDownloadStatusModule::checkLrcValid()
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        return;
    }

    if(checkSettingParameterValue())
    {
        ///Check there is no opening lyrics display mode
       if(m_parentClass->checkMusicListCurrentIndex())
       {
           return;
       }

       const QString &fileName = m_parentClass->currentFileName();
       ///Check if the file exists
       if(QFile::exists(MusicUtils::String::lrcDirPrefix() + fileName + LRC_FILE))
       {
           return;
       }

       ///Start the request query
       MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
       d->setQueryLite(true);
       d->setQueryAllRecords(false);
       d->startToSearch(MusicAbstractQueryRequest::QueryType::Music, fileName);
       connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(currentLrcDataDownload()));
    }
}

void MusicDownloadStatusModule::currentLrcDataDownload()
{
    MusicAbstractQueryRequest *d = TTKObject_cast(MusicAbstractQueryRequest*, sender());
    if(!G_NETWORK_PTR->isOnline() || !d)   //no network connection
    {
        return;
    }

    const MusicObject::MusicSongInformationList songInfos(d->songInfoList());
    if(!songInfos.isEmpty())
    {
        const QString &fileName = d->queryValue();
        const int count = MusicUtils::String::stringSplit(fileName).count();
        const QString &artistName = MusicUtils::String::artistName(fileName);
        const QString &songName = MusicUtils::String::songName(fileName);

        MusicObject::MusicSongInformation info = songInfos.front();
        for(const MusicObject::MusicSongInformation &var : qAsConst(songInfos))
        {
            if(var.m_singerName.contains(artistName, Qt::CaseInsensitive) && var.m_songName.contains(songName, Qt::CaseInsensitive))
            {
                info = var;
                break;
            }
        }

        ///download lrc
        G_DOWNLOAD_QUERY_PTR->makeLrcRequest(info.m_lrcUrl, MusicUtils::String::lrcDirPrefix() + fileName + LRC_FILE, this)->startRequest();
        ///download art picture
        G_DOWNLOAD_QUERY_PTR->makeCoverRequest(info.m_coverUrl, ART_DIR_FULL + artistName + SKN_FILE, this)->startRequest();
        ///download big picture
        G_DOWNLOAD_QUERY_PTR->makeBackgroundRequest(count == 1 ? info.m_singerName : artistName, artistName, this)->startRequest();
    }
    else
    {
        showDownLoadInfoFinished("find error");
    }
}
