#include "musicconnectionpool.h"
#include "musicdownloadstatusmodule.h"
#include "musicplayer.h"
#include "musiclrcmakerwidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicmobilesongsmanagerwidget.h"
#include "musicequalizerdialog.h"
#include "musiclrclocallinkwidget.h"
#include "musicsongslistplaywidget.h"
#include "musicvideoqualitypopwidget.h"
#include "musicvideosearchtablewidget.h"
#include "musicdownloadmgmtwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicsoundeffectswidget.h"
#include "musicsongchecktoolstablewidget.h"
#include "musicitemquerytablewidget.h"
#include "musicabstractdownloadtablewidget.h"
#include "musiccloudtablewidget.h"
#include "musiccloudmanagerwidget.h"
#include "musiclocalmanagerwidget.h"
#include "musicsongdlnatransferwidget.h"
#include "musicsongitemselectedareawidget.h"
#include "musicplaylistbackupwidget.h"

MusicConnectionPool::MusicConnectionPool()
{

}

void MusicConnectionPool::removeValue(const QObject *object)
{
    for(auto it = m_parameters.constBegin(); it != m_parameters.constEnd(); ++it)
    {
        if(it.value() == object)
        {
            m_parameters.take(it.key());
            break;
        }
    }
}

void MusicConnectionPool::connect(const QString &from, const QString &to)
{
    const QObject *first = m_parameters.value(from);
    const QObject *second = m_parameters.value(to);
    if(first == nullptr || second == nullptr)
    {
        return;
    }

    if(from == MusicNetworkThread::className() && to == MusicDownloadStatusModule::className())
    {
        QObject::connect(first, SIGNAL(networkConnectionStateChanged(bool)), second, SLOT(networkConnectionStateChanged(bool)));
    }
    else if(from == MusicPlayer::className() && to == MusicLrcMakerWidget::className())
    {
        QObject::connect(first, SIGNAL(positionChanged(qint64)), second, SLOT(positionChanged(qint64)));
        QObject::connect(first, SIGNAL(durationChanged(qint64)), second, SLOT(durationChanged(qint64)));
    }
    else if((from == MusicLocalManagerWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicMobileSongsManagerWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicSongCheckToolsDuplicateTableWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicSongCheckToolsQualityTableWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicPlaylistBackupTableWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicAbstractDownloadTableWidget::className() && to == MusicSongsSummariziedWidget::className()))
    {
        QObject::connect(first, SIGNAL(addSongToPlaylist(QStringList)), second, SLOT(addSongToPlaylist(QStringList)));
    }
    else if(from == MusicEqualizerDialog::className() && to == MusicPlayer::className())
    {
        QObject::connect(first, SIGNAL(setEqualizerEffect(TTKIntList)), second, SLOT(setEqualizerEffect(TTKIntList)));
        QObject::connect(first, SIGNAL(setEnabledEffect(bool)), second, SLOT(setEnabledEffect(bool)));
    }
    else if(from == MusicEqualizerDialog::className() && to == MusicSoundEffectsWidget::className())
    {
        QObject::connect(first, SIGNAL(setEnabledEffect(bool)), second, SLOT(equalizerButtonChanged(bool)));
    }
    else if(from == MusicSoundEffectsWidget::className() && to == MusicPlayer::className())
    {
        QObject::connect(first, SIGNAL(setEqualizerConfig()), second, SLOT(setEqualizerConfig()));
    }
    else if(from == MusicQueryTableWidget::className() && to == MusicSongsSummariziedWidget::className())
    {
        QObject::connect(first, SIGNAL(songBufferToPlaylist(MusicResultDataItem)), second, SLOT(addSongBufferToPlaylist(MusicResultDataItem)));
    }
    else if(from == MusicLrcLocalLinkWidget::className() && to == MusicDownloadStatusModule::className())
    {
        QObject::connect(first, SIGNAL(currentLrcChanged(QString)), second, SLOT(showDownLoadInfoFinished(QString)));
    }
    else if(from == MusicVideoQualityPopWidget::className() && to == MusicVideoSearchTableWidget::className())
    {
        QObject::connect(first, SIGNAL(queryMediaProps(TTK::MusicSongPropertyList&)), second, SLOT(queryMediaProps(TTK::MusicSongPropertyList&)));
    }
    else if((from == MusicConnectTransferWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicSongItemSelectedAreaWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicSongDlnaTransferWidget::className() && to == MusicSongsSummariziedWidget::className()))
    {
        QObject::connect(first, SIGNAL(queryMusicItemList(MusicSongItemList&)), second, SLOT(queryMusicItemList(MusicSongItemList&)));
    }
    else if((from == MusicCloudManagerTableWidget::className() && to == MusicCloudUploadTableWidget::className()))
    {
        QObject::connect(first, SIGNAL(uploadFileError(MusicCloudDataItem)), second, SLOT(uploadFileError(MusicCloudDataItem)));
        QObject::connect(second, SIGNAL(reuploadFilesToServer(QStringList)), first, SLOT(reuploadFilesToServer(QStringList)));
    }
}
