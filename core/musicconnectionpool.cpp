#include "musicconnectionpool.h"

#include <QWidget>
#include "musicobject.h"

MusicConnectionPool::MusicConnectionPool(QObject *parent)
    : QObject(parent)
{

}

void MusicConnectionPool::setNetworkMultiValue(QObject *object)
{
    m_queueList << object;
    QObject *to = m_para.value( "MusicDownloadStatusLabel" );
    if(to != NULL)
    {
        QObject::connect(object, SIGNAL(musicDownLoadFinished(QString)), to,
                                 SLOT(showDownLoadInfoFinished(QString)));
    }
}

void MusicConnectionPool::connectMusicDownload(QObject *object)
{
    QObject *to = m_para.value( "MusicMyDownloadRecordWidget" );
    if(to != NULL && object)
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

void MusicConnectionPool::connect(const QString &from,
                                  const QString &to)
{
    QObject *first = m_para.value(from);
    QObject *second = m_para.value(to);
    if(first == NULL || second == NULL)
    {
        return;
    }

    if(from == "MusicSpectrumWidget" && to == "MusicPlayer" )
    {
        QObject::connect(first, SIGNAL(setSpectrum(HWND,int,int)), second,
                                SLOT(setSpectrum(HWND,int,int)));
    }

    if( (from == "MusicLocalSongsManagerWidget" && to == "MusicApplication") ||
        (from == "MusicMyDownloadRecordWidget" && to == "MusicApplication") )
    {
        QObject::connect(first, SIGNAL(addSongToPlay(QStringList)), second,
                                SLOT(addSongToPlayList(QStringList)));
    }

    if(from == "MusicTimerWidget" && to == "MusicApplicationObject" )
    {
        QObject::connect(first, SIGNAL(timerParameterChanged()), second,
                                SLOT(musicToolSetsParameter()));
    }

    if( (from == "MusicToolSetsWidget" && to == "MusicApplication") ||
        (from == "MusicApplicationObject" && to == "MusicApplication") )
    {
        QObject::connect(first, SIGNAL(getCurrentPlayList(QStringList&)), second,
                                SLOT(getCurrentPlayList(QStringList&)));
    }

    if(from == "MusicEqualizerDialog" && to == "MusicPlayer" )
    {
        QObject::connect(first, SIGNAL(setEqEffect(MIntList)), second,
                                SLOT(setEqEffect(MIntList)));
        QObject::connect(first, SIGNAL(setEnaleEffect(bool)), second,
                                SLOT(setEnaleEffect(bool)));
        QObject::connect(first, SIGNAL(setSpEqEffect(MusicObject::SpecialEQ)), second,
                                SLOT(setSpEqEffect(MusicObject::SpecialEQ)));
        QObject::connect(first, SIGNAL(resetEqEffect()), second,
                                SLOT(resetEqEffect()));
    }

    if(from == "MusicSongSearchOnlineTableWidget" && to == "MusicDownloadStatusLabel" )
    {
        QObject::connect(first, SIGNAL(showDownLoadInfoFor(MusicObject::DownLoadType)), second,
                                SLOT(showDownLoadInfoFor(MusicObject::DownLoadType)));
    }

    if(from == "MusicBgThemeDownload" && to == "MusicTopAreaWidget" )
    {
        QObject::connect(first, SIGNAL(musicBgDownloadFinished()), second,
                                SLOT(musicBgThemeDownloadFinished()));
    }

    if(from == "MusicSongSearchOnlineTableWidget" && to == "MusicSongsSummarizied" )
    {
        QObject::connect(first, SIGNAL(muiscSongToPlayListChanged(QString)), second,
                                SLOT(addNetMusicSongToList(QString)));
    }

    if(from == "MusicSongSearchOnlineTableWidget" && to == "MusicRightAreaWidget" )
    {
        QObject::connect(first, SIGNAL(restartSearchQuery(QString)), second,
                                SLOT(musicResearchButtonSearched(QString)));
    }

    if(from == "MusicNetworkThread" && to == "MusicDownloadStatusLabel" )
    {
        QObject::connect(first, SIGNAL(networkConnectionStateChanged(bool)), second,
                                SLOT(networkConnectionStateChanged(bool)));
    }

    if(from == "MusicSongSearchOnlineTableWidget" && to == "MusicQualityChoiceTableWidget" )
    {
        QObject::connect(first, SIGNAL(getQualityString(QString&)), second,
                                SLOT(getQualityString(QString&)));
        QObject::connect(second, SIGNAL(researchQueryByQuality()), first,
                                 SLOT(researchQueryByQuality()));
    }

    if(from == "MusicSongsListPlayWidget" && to == "MusicRightAreaWidget" )
    {
        QObject::connect(first, SIGNAL(renameFinished(QString)), second,
                                SLOT(musicVideoButtonSearched(QString)));
    }

    if(from == "MusicVideoControl" && to == "MusicRightAreaWidget" )
    {
        QObject::connect(first, SIGNAL(musicVideoSetPopup(bool)), second,
                                SLOT(musicVideoSetPopup(bool)));
        QObject::connect(first, SIGNAL(musicVideoFullscreen(bool)), second,
                                SLOT(musicVideoFullscreen(bool)));
    }

    if(from == "MusicApplicationObject" && to == "MusicEnhancedWidget" )
    {
        QObject::connect(first, SIGNAL(enhancedMusicChanged(int)), second,
                                SLOT(setEnhancedMusicConfig(int)));
    }

}

void MusicConnectionPool::disConnect(const QString &name)
{
    m_para.take(name);
}
