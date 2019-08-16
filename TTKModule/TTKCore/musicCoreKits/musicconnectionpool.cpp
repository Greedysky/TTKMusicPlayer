#include "musicconnectionpool.h"
#include "musicobject.h"
#include "musicsong.h"
#include "musicdownloadstatusobject.h"
#include "musicnetworkthread.h"
#ifndef MUSIC_MOBILE
#include "musicplayer.h"
#include "musiclrcmakerwidget.h"
#include "musicsongssummariziedwidget.h"
#include "musiclocalsongsmanagerwidget.h"
#include "musicequalizerdialog.h"
#include "musicsongsearchonlinewidget.h"
#include "musiclrclocallinkwidget.h"
#include "musicsongslistplaywidget.h"
#include "musicvideoqualitypopwidget.h"
#include "musicvideosearchtablewidget.h"
#include "musicdownloadmgmtwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicsoundeffectswidget.h"
#include "musicsongchecktoolswidget.h"
#include "musicsongchecktoolstablewidget.h"
#include "musicqueryfoundtablewidget.h"
#include "musicdownloadabstracttablewidget.h"
#include "musiccloudtablewidget.h"
#include "musiccloudmanagerwidget.h"
#include "musiclrcdownloadbatchwidget.h"
#endif

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

    if(from == MusicNetworkThread::getClassName() && to == MusicDownloadStatusObject::getClassName())
    {
        QObject::connect(first, SIGNAL(networkConnectionStateChanged(bool)), second, SLOT(networkConnectionStateChanged(bool)));
    }
#ifndef MUSIC_MOBILE
    else if(from == MusicPlayer::getClassName() && to == MusicLrcMakerWidget::getClassName())
    {
        QObject::connect(first, SIGNAL(positionChanged(qint64)), second, SLOT(positionChanged(qint64)));
        QObject::connect(first, SIGNAL(durationChanged(qint64)), second, SLOT(durationChanged(qint64)));
    }
    else if((from == MusicLocalSongsManagerWidget::getClassName() && to == MusicSongsSummariziedWidget::getClassName()) ||
            (from == MusicSongCheckToolsDuplicateTableWidget::getClassName() && to == MusicSongsSummariziedWidget::getClassName()) ||
            (from == MusicSongCheckToolsQualityTableWidget::getClassName() && to == MusicSongsSummariziedWidget::getClassName()) ||
            (from == MusicDownloadAbstractTableWidget::getClassName() && to == MusicSongsSummariziedWidget::getClassName()))
    {
        QObject::connect(first, SIGNAL(addSongToPlay(QStringList)), second, SLOT(addSongToPlaylist(QStringList)));
    }
    else if(from == MusicEqualizerDialog::getClassName() && to == MusicPlayer::getClassName())
    {
        QObject::connect(first, SIGNAL(setEqEffect(MIntList)), second, SLOT(setEqEffect(MIntList)));
        QObject::connect(first, SIGNAL(setEnaleEffect(bool)), second, SLOT(setEnaleEffect(bool)));
    }
    else if(from == MusicEqualizerDialog::getClassName() && to == MusicSoundEffectsWidget::getClassName())
    {
        QObject::connect(first, SIGNAL(setEnaleEffect(bool)), second, SLOT(equalizerButtonChanged(bool)));
    }
    else if(from == MusicSoundEffectsWidget::getClassName() && to == MusicPlayer::getClassName())
    {
        QObject::connect(first, SIGNAL(volumeChanged(int)), second, SLOT(setSoundEffectVolume(int)));
        QObject::connect(first, SIGNAL(setEqInformation()), second, SLOT(setEqInformation()));
    }
    else if(from == MusicSongSearchTableWidget::getClassName() && to == MusicDownloadStatusObject::getClassName())
    {
        QObject::connect(first, SIGNAL(showDownLoadInfoFor(MusicObject::DownLoadMode)), second, SLOT(showDownLoadInfoFor(MusicObject::DownLoadMode)));
    }
    else if((from == MusicSongSearchTableWidget::getClassName() && to == MusicSongsSummariziedWidget::getClassName()) ||
            (from == MusicQueryFoundTableWidget::getClassName() && to == MusicSongsSummariziedWidget::getClassName()))
    {
        QObject::connect(first, SIGNAL(musicSongToPlaylistChanged(QString,QString,QString,bool)), second, SLOT(addNetMusicSongToList(QString,QString,QString,bool)));
    }
    else if(from == MusicLrcLocalLinkWidget::getClassName() && to == MusicDownloadStatusObject::getClassName())
    {
        QObject::connect(first, SIGNAL(currentLrcChanged(QString)), second, SLOT(showDownLoadInfoFinished(QString)));
    }
    else if(from == MusicVideoQualityPopWidget::getClassName() && to == MusicVideoSearchTableWidget::getClassName())
    {
        QObject::connect(first, SIGNAL(getMusicMediaInfo(MusicObject::MusicSongAttributes&)), second, SLOT(getMusicMediaInfo(MusicObject::MusicSongAttributes&)));
    }
    else if((from == MusicConnectTransferWidget::getClassName() && to == MusicSongsSummariziedWidget::getClassName()) ||
            (from == MusicSongCheckToolsWidget::getClassName() && to == MusicSongsSummariziedWidget::getClassName()) ||
            (from == MusicLrcDownloadBatchWidget::getClassName() && to == MusicSongsSummariziedWidget::getClassName()))
    {
        QObject::connect(first, SIGNAL(getMusicLists(MusicSongItems&)), second, SLOT(getMusicLists(MusicSongItems&)));
    }
    else if((from == MusicCloudManagerTableWidget::getClassName() && to == MusicCloudUploadTableWidget::getClassName()))
    {
        QObject::connect(first, SIGNAL(uploadFileError(MusicCloudDataItem)), second, SLOT(uploadFileError(MusicCloudDataItem)));
        QObject::connect(second, SIGNAL(reuploadFilesToServer(QStringList)), first, SLOT(reuploadFilesToServer(QStringList)));
    }
#endif
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
