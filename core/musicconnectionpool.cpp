#include "musicconnectionpool.h"

#include <QWidget>
#include "musicobject.h"

MusicConnectionPool::MusicConnectionPool(QObject *parent)
    : QObject(parent)
{

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

    if(from == "MusicDownLoadThreadAbstract" && to == "MusicDownloadStatusLabel" )
    {
        QObject::connect(first, SIGNAL(musicDownLoadFinished(QString)), second,
                                SLOT(showDownLoadInfoFinished(QString)));
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
}

void MusicConnectionPool::disConnect(const QString &name)
{
    m_para.take(name);
}
