#include "musicdownloadstatusmodule.h"
#include "musicabstractqueryrequest.h"
#include "musicapplication.h"
#include "musicbottomareawidget.h"
#include "musicconnectionpool.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadbackgroundrequest.h"

MusicDownloadStatusModule::MusicDownloadStatusModule(QObject *parent)
    : QObject(parent),
      m_previousState(true),
      m_parent(TTKObject_cast(MusicApplication*, parent))
{
    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(MusicNetworkThread::className(), className());
}

void MusicDownloadStatusModule::checkMetaDataValid(bool mode)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        return;
    }

    ///Check there is no opening lyrics display mode
    if(m_parent->checkMusicListCurrentIndex())
    {
        return;
    }

    if(!mode && checkLrcValid() && checkArtistCoverValid() && checkArtistBackgroundValid())
    {
        return;
    }

    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
    d->setQueryLite(true);
    d->setQueryAllRecords(false);
    d->setHeader("mode", mode);
    d->startToSearch(MusicAbstractQueryRequest::QueryType::Music, m_parent->currentFileName());
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(currentMetaDataDownload()));
}

void MusicDownloadStatusModule::currentMetaDataDownload()
{
    MusicAbstractQueryRequest *d = TTKObject_cast(MusicAbstractQueryRequest*, sender());
    if(!G_NETWORK_PTR->isOnline() || !d)   //no network connection
    {
        return;
    }

    const MusicObject::MusicSongInformationList songInfos(d->songInfoList());
    if(songInfos.isEmpty())
    {
        showDownLoadInfoFinished("find error");
        return;
    }

    const QString &fileName = d->queryValue();
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

    const bool mode = d->header("mode").toBool();
    if(mode || !checkLrcValid())
    {
        ///download lrc
        G_DOWNLOAD_QUERY_PTR->makeLrcRequest(info.m_lrcUrl, MusicUtils::String::lrcDirPrefix() + fileName + LRC_FILE, this)->startRequest();
    }

    if(mode || !checkArtistCoverValid())
    {
        ///download art picture
        G_DOWNLOAD_QUERY_PTR->makeCoverRequest(info.m_coverUrl, ART_DIR_FULL + artistName + SKN_FILE, this)->startRequest();
    }

    if(mode || !checkArtistBackgroundValid())
    {
        ///download art background picture
        const int count = MusicUtils::String::stringSplit(fileName).count();
        G_DOWNLOAD_QUERY_PTR->makeBackgroundRequest(count == 1 ? info.m_singerName : artistName, artistName, this)->startRequest();
    }
}

void MusicDownloadStatusModule::showDownLoadInfoFinished(const QString &bytes)
{
    ///If the lyrics download finished immediately loaded to display
    if(bytes == DOWNLOAD_KEY_LRC)
    {
        m_parent->musicLoadCurrentSongLrc();
    }
    else if(bytes == DOWNLOAD_KEY_COVER)
    {
        m_parent->updateCurrentArtist();
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

bool MusicDownloadStatusModule::checkLrcValid() const
{
    const QString &fileName = m_parent->currentFileName();
    return QFile::exists(MusicUtils::String::lrcDirPrefix() + fileName + LRC_FILE);
}

bool MusicDownloadStatusModule::checkArtistCoverValid() const
{
    const QString &fileName = MusicUtils::String::artistName(m_parent->currentFileName());
    return QFile::exists(ART_DIR_FULL + fileName + SKN_FILE);
}

bool MusicDownloadStatusModule::checkArtistBackgroundValid() const
{
    const QString &fileName = MusicUtils::String::artistName(m_parent->currentFileName());
    return QFile::exists(BACKGROUND_DIR_FULL + fileName + "0" + SKN_FILE);
}
