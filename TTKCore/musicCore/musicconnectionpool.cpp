#include "musicconnectionpool.h"
#include "musicobject.h"
#include "musicsong.h"
#include "musicdownloadstatuslabel.h"
#include "musicnetworkthread.h"
#ifndef MUSIC_MOBILE
#include "musicmydownloadrecordwidget.h"
#include "musicplayer.h"
#include "musiclrcmakerwidget.h"
#include "musicsongssummarizied.h"
#include "musiclocalsongsmanagerwidget.h"
#include "musicequalizerdialog.h"
#include "musicsongsearchonlinewidget.h"
#include "musicsimilarfoundwidget.h"
#include "musiclrclocallinkwidget.h"
#include "musicsongslistplaywidget.h"
#include "musicvideoqualitypopwidget.h"
#include "musicvideotablewidget.h"
#include "musicdownloadmgmtwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicalbumfoundwidget.h"
#include "musicartistfoundwidget.h"
#include "musicsoundeffectswidget.h"
#endif

MusicConnectionPool::MusicConnectionPool()
    : QObject(nullptr)
{

}

QString MusicConnectionPool::getClassName()
{
    return staticMetaObject.className();
}

void MusicConnectionPool::setNetworkMultiValue(QObject *object)
{
    m_queueList << object;
    QObject *to = m_para.value( MusicDownloadStatusLabel::getClassName() );
    if(to != nullptr)
    {
        QObject::connect(object, SIGNAL(downLoadDataChanged(QString)), to,
                                 SLOT(showDownLoadInfoFinished(QString)));
    }
}

void MusicConnectionPool::connectMusicDownload(QObject *object)
{
#ifndef MUSIC_MOBILE
    QObject *to = m_para.value( MusicMyDownloadRecordWidget::getClassName() );
    if(to != nullptr && object)
    {
        QObject::connect(object, SIGNAL(downloadProgressChanged(float, QString, qint64)), to,
                                 SLOT(downloadProgressChanged(float, QString, qint64)));
        QObject::connect(object, SIGNAL(createDownloadItem(QString, qint64)), to,
                                 SLOT(createDownloadItem(QString, qint64)));
    }
#else
    Q_UNUSED(object);
#endif
}

void MusicConnectionPool::removeNetworkMultiValue(QObject *object)
{
    int index = m_queueList.indexOf(object);
    if(index != -1)
    {
        m_queueList.takeAt( index );
    }
}

void MusicConnectionPool::poolConnect(const QString &from, const QString &to)
{
    QObject *first = m_para.value(from);
    QObject *second = m_para.value(to);
    if(first == nullptr || second == nullptr)
    {
        return;
    }

    if(from == MusicNetworkThread::getClassName() && to == MusicDownloadStatusLabel::getClassName() )
    {
        QObject::connect(first, SIGNAL(networkConnectionStateChanged(bool)), second,
                                SLOT(networkConnectionStateChanged(bool)));
    }
#ifndef MUSIC_MOBILE
    else if(from == MusicPlayer::getClassName() && to == MusicLrcMakerWidget::getClassName() )
    {
        QObject::connect(first, SIGNAL(positionChanged(qint64)), second,
                                SLOT(positionChanged(qint64)));
        QObject::connect(first, SIGNAL(durationChanged(qint64)), second,
                                SLOT(durationChanged(qint64)));
    }
    else if( (from == MusicLocalSongsManagerWidget::getClassName() && to == MusicSongsSummarizied::getClassName() ) ||
             (from == MusicMyDownloadRecordWidget::getClassName() && to == MusicSongsSummarizied::getClassName()) )
    {
        QObject::connect(first, SIGNAL(addSongToPlay(QStringList)), second,
                                SLOT(addSongToPlayList(QStringList)));
    }
    else if(from == MusicEqualizerDialog::getClassName() && to == MusicPlayer::getClassName() )
    {
        QObject::connect(first, SIGNAL(setEqEffect(MusicObject::MIntList)), second,
                                SLOT(setEqEffect(MusicObject::MIntList)));
        QObject::connect(first, SIGNAL(setEnaleEffect(bool)), second,
                                SLOT(setEnaleEffect(bool)));
    }
    else if(from == MusicEqualizerDialog::getClassName() && to == MusicSoundEffectsWidget::getClassName() )
    {
        QObject::connect(first, SIGNAL(setEnaleEffect(bool)), second,
                                SLOT(equalizerButtonChanged(bool)));
    }
    else if(from == MusicSoundEffectsWidget::getClassName() && to == MusicPlayer::getClassName() )
    {
        QObject::connect(first, SIGNAL(volumeChanged(int)), second,
                                SLOT(setSoundEffectVolume(int)));
        QObject::connect(first, SIGNAL(setEqInformation()), second,
                                SLOT(setEqInformation()));
    }
    else if(from == MusicSongSearchOnlineTableWidget::getClassName() && to == MusicDownloadStatusLabel::getClassName() )
    {
        QObject::connect(first, SIGNAL(showDownLoadInfoFor(MusicObject::DownLoadType)), second,
                                SLOT(showDownLoadInfoFor(MusicObject::DownLoadType)));
    }
    else if((from == MusicSongSearchOnlineTableWidget::getClassName() && to == MusicSongsSummarizied::getClassName()) ||
            (from == MusicSimilarFoundTableWidget::getClassName() && to == MusicSongsSummarizied::getClassName()) ||
            (from == MusicAlbumFoundTableWidget::getClassName() && to == MusicSongsSummarizied::getClassName()) ||
            (from == MusicArtistFoundTableWidget::getClassName() && to == MusicSongsSummarizied::getClassName()) )
    {
        QObject::connect(first, SIGNAL(muiscSongToPlayListChanged(QString,QString,QString,bool)), second,
                                SLOT(addNetMusicSongToList(QString,QString,QString,bool)));
    }
    else if(from == MusicLrcLocalLinkWidget::getClassName() && to == MusicDownloadStatusLabel::getClassName() )
    {
        QObject::connect(first, SIGNAL(currentLrcChanged(QString)), second,
                                SLOT(showDownLoadInfoFinished(QString)));
    }
    else if(from == MusicVideoQualityPopWidget::getClassName() && to == MusicVideoTableWidget::getClassName() )
    {
        QObject::connect(first, SIGNAL(getMusicMvInfo(MusicObject::MusicSongAttributes&)), second,
                                SLOT(getMusicMvInfo(MusicObject::MusicSongAttributes&)));
    }
    else if(from == MusicConnectTransferWidget::getClassName() && to == MusicSongsSummarizied::getClassName() )
    {
        QObject::connect(first, SIGNAL(getMusicLists(MusicSongItems&)), second,
                                SLOT(getMusicLists(MusicSongItems&)));
    }
#endif
}

void MusicConnectionPool::poolConnect(const QObject *from, const QObject *to)
{
    QObject::connect(from, SIGNAL(sender()), to, SLOT(receiver()));
}

void MusicConnectionPool::removeValue(const QString &name)
{
    m_para.take(name);
}

void MusicConnectionPool::removeValue(const QObject *object)
{
    QMapIterator<QString, QObject*> i(m_para);
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
