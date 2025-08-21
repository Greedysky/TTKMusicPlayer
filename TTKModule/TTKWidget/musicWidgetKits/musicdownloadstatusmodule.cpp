#include "musicdownloadstatusmodule.h"
#include "musicabstractqueryrequest.h"
#include "musicapplication.h"
#include "musicbottomareawidget.h"
#include "musicconnectionpool.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadbackgroundrequest.h"
#include "musicsong.h"

MusicDownloadStatusModule::MusicDownloadStatusModule(QObject *parent)
    : QObject(parent),
      m_previousState(true),
      m_parent(TTKObjectCast(MusicApplication*, parent))
{
    G_CONNECTION_PTR->setValue(className(), this);
    connect(G_NETWORK_PTR, SIGNAL(networkConnectionStateChanged(bool)), this, SLOT(networkConnectionStateChanged(bool)));
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
    d->setHeader("mode", mode);
    d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    d->startToSearch(m_parent->currentFileName());
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(currentMetaDataDownload()));
}

void MusicDownloadStatusModule::currentMetaDataDownload()
{
    MusicAbstractQueryRequest *d = TTKObjectCast(MusicAbstractQueryRequest*, sender());
    if(!G_NETWORK_PTR->isOnline() || !d)   //no network connection
    {
        return;
    }

    const TTK::MusicSongInformationList &songInfos = d->items();
    if(songInfos.isEmpty())
    {
        showDownLoadInfoFinished("find error");
        return;
    }

    const QString &fileName = d->queryValue();
    const QString &songName = TTK::generateSongTitle(fileName);
    const QString &artistName = TTK::generateSongArtist(fileName);

    TTK::MusicSongInformation info = songInfos.front();
    for(const TTK::MusicSongInformation &var : qAsConst(songInfos))
    {
        if(var.m_artistName.contains(artistName, Qt::CaseInsensitive) && var.m_songName.contains(songName, Qt::CaseInsensitive))
        {
            info = var;
            break;
        }
    }

    const bool mode = d->header("mode").toBool();
    if(mode || !checkLrcValid())
    {
        ///download lrc
        G_DOWNLOAD_QUERY_PTR->makeLrcRequest(info.m_lrcUrl, TTK::String::lrcDirPrefix() + fileName + LRC_FILE, this)->startToRequest();
    }

    if(mode || !checkArtistCoverValid())
    {
        ///download art picture
        G_DOWNLOAD_QUERY_PTR->makeCoverRequest(info.m_coverUrl, ART_DIR_FULL + artistName + SKN_FILE, this)->startToRequest();
    }

    if(mode || !checkArtistBackgroundValid())
    {
        ///download art background picture
        const int count = TTK::String::split(fileName).count();
        G_DOWNLOAD_QUERY_PTR->makeBackgroundRequest(count == 1 ? info.m_artistName : artistName, artistName, this)->startToRequest();
    }
}

void MusicDownloadStatusModule::showDownLoadInfoFinished(const QString &bytes)
{
    ///lyrics download finished immediately loaded to display
    if(bytes == DOWNLOAD_KEY_LRC)
    {
        m_parent->loadCurrentSongLrc();
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
    return QFile::exists(TTK::String::lrcDirPrefix() + fileName + LRC_FILE);
}

bool MusicDownloadStatusModule::checkArtistCoverValid() const
{
    const QString &fileName = TTK::generateSongArtist(m_parent->currentFileName());
    return QFile::exists(ART_DIR_FULL + fileName + SKN_FILE);
}

bool MusicDownloadStatusModule::checkArtistBackgroundValid() const
{
    const QString &fileName = TTK::generateSongArtist(m_parent->currentFileName());
    return QFile::exists(BACKGROUND_DIR_FULL + fileName + "-0" + SKN_FILE);
}
