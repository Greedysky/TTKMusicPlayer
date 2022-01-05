#include "musicconnectionpool.h"
#include "musicdownloadstatusmodule.h"
#include "musicplayer.h"
#include "musiclrcmakerwidget.h"
#include "musicsongssummariziedwidget.h"
#include "musiclocalsongsmanagerwidget.h"
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
#include "musicsongdlnatransferwidget.h"
#include "musicsongitemselectedareawidget.h"

MusicConnectionPool::MusicConnectionPool()
{

}

void MusicConnectionPool::poolConnect(const QString &from, const QString &to)
{
    const QObject *first = m_parameter.value(from);
    const QObject *second = m_parameter.value(to);
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
    else if((from == MusicLocalSongsManagerWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicSongCheckToolsDuplicateTableWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicSongCheckToolsQualityTableWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
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
        QObject::connect(first, SIGNAL(setEqInformation()), second, SLOT(setEqInformation()));
    }
    else if((from == MusicSongSearchTableWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicItemQueryTableWidget::className() && to == MusicSongsSummariziedWidget::className()))
    {
        QObject::connect(first, SIGNAL(musicSongToPlaylistChanged(MusicResultsItem)), second, SLOT(addNetMusicSongToPlaylist(MusicResultsItem)));
    }
    else if(from == MusicLrcLocalLinkWidget::className() && to == MusicDownloadStatusModule::className())
    {
        QObject::connect(first, SIGNAL(currentLrcChanged(QString)), second, SLOT(showDownLoadInfoFinished(QString)));
    }
    else if(from == MusicVideoQualityPopWidget::className() && to == MusicVideoSearchTableWidget::className())
    {
        QObject::connect(first, SIGNAL(queryMusicMediaInfo(MusicObject::MusicSongAttributes&)), second, SLOT(musicMediaInfo(MusicObject::MusicSongAttributes&)));
    }
    else if((from == MusicConnectTransferWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicSongItemSelectedAreaWidget::className() && to == MusicSongsSummariziedWidget::className()) ||
            (from == MusicSongDlnaTransferWidget::className() && to == MusicSongsSummariziedWidget::className()))
    {
        QObject::connect(first, SIGNAL(queryMusicItemList(MusicSongItems&)), second, SLOT(musicItemList(MusicSongItems&)));
    }
    else if((from == MusicCloudManagerTableWidget::className() && to == MusicCloudUploadTableWidget::className()))
    {
        QObject::connect(first, SIGNAL(uploadFileError(MusicCloudDataItem)), second, SLOT(uploadFileError(MusicCloudDataItem)));
        QObject::connect(second, SIGNAL(reuploadFilesToServer(QStringList)), first, SLOT(reuploadFilesToServer(QStringList)));
    }
}

void MusicConnectionPool::poolConnect(const QObject *from, const QObject *to)
{
    QObject::connect(from, SIGNAL(sender()), to, SLOT(receiver()));
}

void MusicConnectionPool::removeValue(const QString &name)
{
    m_parameter.take(name);
}

void MusicConnectionPool::removeValue(const QObject *object)
{
    QMapIterator<QString, QObject*> i(m_parameter);
    while(i.hasNext())
    {
        i.next();
        if(i.value() == object)
        {
            removeValue(i.key());
            break;
        }
    }
}
