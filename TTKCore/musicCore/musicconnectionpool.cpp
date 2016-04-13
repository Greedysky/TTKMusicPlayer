#include "musicconnectionpool.h"
#include "musicobject.h"
#include "musicsong.h"

#include <QWidget>

MusicConnectionPool::MusicConnectionPool()
    : QObject(nullptr)
{

}

void MusicConnectionPool::setNetworkMultiValue(QObject *object)
{
    m_queueList << object;
    QObject *to = m_para.value( "MusicDownloadStatusLabel" );
    if(to != nullptr)
    {
        QObject::connect(object, SIGNAL(musicDownLoadFinished(QString)), to,
                                 SLOT(showDownLoadInfoFinished(QString)));
    }
}

void MusicConnectionPool::connectMusicDownload(QObject *object)
{
    QObject *to = m_para.value( "MusicMyDownloadRecordWidget" );
    if(to != nullptr && object)
    {
        QObject::connect(object, SIGNAL(downloadProgressChanged(float, QString, qint64)), to,
                                 SLOT(downloadProgressChanged(float, QString, qint64)));
        QObject::connect(object, SIGNAL(createDownloadItem(QString, qint64)), to,
                                 SLOT(createDownloadItem(QString, qint64)));
    }
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

    if(from == "MusicPlayer" && to == "MusicLrcMakerWidget" )
    {
        QObject::connect(first, SIGNAL(positionChanged(qint64)), second,
                                SLOT(setCurrentPosition(qint64)));
    }
    else if( (from == "MusicLocalSongsManagerWidget" && to == "MusicSongsSummarizied") ||
             (from == "MusicMyDownloadRecordWidget" && to == "MusicSongsSummarizied") )
    {
        QObject::connect(first, SIGNAL(addSongToPlay(QStringList)), second,
                                SLOT(addSongToPlayList(QStringList)));
    }
    else if(from == "MusicTimerWidget" && to == "MusicApplicationObject" )
    {
        QObject::connect(first, SIGNAL(timerParameterChanged()), second,
                                SLOT(musicToolSetsParameter()));
    }
    else if( (from == "MusicToolSetsWidget" && to == "MusicApplication") ||
        (from == "MusicApplicationObject" && to == "MusicApplication") )
    {
        QObject::connect(first, SIGNAL(getCurrentPlayList(QStringList&)), second,
                                SLOT(getCurrentPlayList(QStringList&)));
    }
    else if(from == "MusicToolSetsWidget" && to == "MusicLeftAreaWidget" )
    {
        QObject::connect(first, SIGNAL(showSpectrumWidget()), second,
                                SLOT(musicAnalyzerSpectrumWidget()));
    }
    else if(from == "MusicEqualizerDialog" && to == "MusicPlayer" )
    {
        QObject::connect(first, SIGNAL(setEqEffect(MIntList)), second,
                                SLOT(setEqEffect(MIntList)));
        QObject::connect(first, SIGNAL(setEnaleEffect(bool)), second,
                                SLOT(setEnaleEffect(bool)));
    }
    else if(from == "MusicSongSearchOnlineTableWidget" && to == "MusicDownloadStatusLabel" )
    {
        QObject::connect(first, SIGNAL(showDownLoadInfoFor(MusicObject::DownLoadType)), second,
                                SLOT(showDownLoadInfoFor(MusicObject::DownLoadType)));
    }
    else if(from == "MusicBackgroundDownload" && to == "MusicTopAreaWidget" )
    {
        QObject::connect(first, SIGNAL(musicBgDownloadFinished()), second,
                                SLOT(musicBgThemeDownloadFinished()));
    }
    else if(from == "MusicSongSearchOnlineTableWidget" && to == "MusicSongsSummarizied" )
    {
        QObject::connect(first, SIGNAL(muiscSongToPlayListChanged(QString,QString,QString)), second,
                                SLOT(addNetMusicSongToList(QString,QString,QString)));
    }
    else if(from == "MusicSongSearchOnlineTableWidget" && to == "MusicRightAreaWidget" )
    {
        QObject::connect(first, SIGNAL(restartSearchQuery(QString)), second,
                                SLOT(songResearchButtonSearched(QString)));
    }
    else if(from == "MusicNetworkThread" && to == "MusicDownloadStatusLabel" )
    {
        QObject::connect(first, SIGNAL(networkConnectionStateChanged(bool)), second,
                                SLOT(networkConnectionStateChanged(bool)));
    }
    else if(from == "MusicSongSearchOnlineTableWidget" && to == "MusicQualityChoiceTableWidget" )
    {
        QObject::connect(first, SIGNAL(getQualityString(QString&)), second,
                                SLOT(getQualityString(QString&)));
        QObject::connect(second, SIGNAL(researchQueryByQuality()), first,
                                 SLOT(researchQueryByQuality()));
    }
    else if(from == "MusicSongsListPlayWidget" && to == "MusicRightAreaWidget" )
    {
        QObject::connect(first, SIGNAL(videoButtonClicked(QString)), second,
                                SLOT(musicVideoButtonSearched(QString)));
    }
    else if(from == "MusicSongsListPlayWidget" && to == "MusicApplication" )
    {
        QObject::connect(first, SIGNAL(currentLoveStateChanged()), second,
                                SLOT(musicAddSongToLovestListAt()));
    }
    else if(from == "MusicLeftAreaWidget" && to == "MusicSongsListPlayWidget" )
    {
        QObject::connect(second, SIGNAL(currentDownloadStateChanged()), first,
                                SLOT(musicDownloadSongToLocal()));
        QObject::connect(first, SIGNAL(currentLoveStateChanged()), second,
                                SLOT(currentLoveStateClicked()));
        QObject::connect(first, SIGNAL(currentDownloadStateChanged()), second,
                                SLOT(currentDownloadStateClicked()));
    }
    else if(from == "MusicVideoControl" && to == "MusicRightAreaWidget" )
    {
        QObject::connect(first, SIGNAL(musicVideoSetPopup(bool)), second,
                                SLOT(musicVideoSetPopup(bool)));
        QObject::connect(first, SIGNAL(musicVideoFullscreen(bool)), second,
                                SLOT(musicVideoFullscreen(bool)));
    }
    else if(from == "MusicApplicationObject" && to == "MusicEnhancedWidget" )
    {
        QObject::connect(first, SIGNAL(enhancedMusicChanged(int)), second,
                                SLOT(setEnhancedMusicConfig(int)));
    }
    else if(from == "MusicVideoControl" && to == "MusicVideoTableWidget" )
    {
        QObject::connect(first, SIGNAL(getMusicMvInfo(MusicSongAttributes&)), second,
                                SLOT(getMusicMvInfo(MusicSongAttributes&)));
        QObject::connect(first, SIGNAL(downloadLocalByControl()), second,
                                SLOT(downloadLocalFromControl()));
    }
    else if(from == "MusicSongsSummariziedFloatWidget" && to == "MusicBottomAreaWidget" )
    {
        QObject::connect(first, SIGNAL(musicSearch()), second,
                                SLOT(musicSearch()));
    }
    else if(from == "MusicSongsSummariziedFloatWidget" && to == "MusicApplication" )
    {
        QObject::connect(first, SIGNAL(musicCurrentPlayLocation()), second,
                                SLOT(musicCurrentPlayLocation()));
    }
    else if(from == "MusicDownloadResetWidget" && to == "MusicLeftAreaWidget" )
    {
        QObject::connect(first, SIGNAL(openStackedDownloadWidget()), second,
                                SLOT(musicStackedMyDownWidgetChanged()));
    }
    else if(from == "MusicConnectTransferWidget" && to == "MusicSongsSummarizied" )
    {
        QObject::connect(first, SIGNAL(getMusicLists(MusicSongsList&,QStringList&)), second,
                                SLOT(getMusicLists(MusicSongsList&,QStringList&)));
    }

}

void MusicConnectionPool::poolConnect(const QObject *from, const QObject *to)
{
    QObject::connect(from, SIGNAL(sender()), to, SLOT(receiver()));
}

void MusicConnectionPool::poolDisConnect(const QString &name)
{
    m_para.take(name);
}

void MusicConnectionPool::poolDisConnect(const QObject *object)
{
    QMapIterator<QString, QObject*> i(m_para);
    while(i.hasNext())
    {
        i.next();
        if(i.value() == object)
        {
            poolDisConnect(i.key());
            break;
        }
    }
}
