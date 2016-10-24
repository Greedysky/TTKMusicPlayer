#include "ttkradiohelper.h"
#include "musicradiochannelthread.h"
#include "musicradioplaylistthread.h"
#include "musicradiosongsthread.h"

//#include <QMediaPlayer>
#include <QNetworkCookieJar>

TTKRadioHelper::TTKRadioHelper(QObject *parent)
    : QObject(parent)
{
    m_cookJar = new QNetworkCookieJar(this);
    m_getChannelThread = nullptr;
    m_playListThread = nullptr;
    m_songsThread = nullptr;
//    m_player = new QMediaPlayer(this);
}

TTKRadioHelper::~TTKRadioHelper()
{
    delete m_getChannelThread;
    delete m_playListThread;
    delete m_songsThread;
    delete m_cookJar;
//    delete m_player;
}

void TTKRadioHelper::init()
{
    m_getChannelThread = new MusicRadioChannelThread(this, m_cookJar);
    connect(m_getChannelThread, SIGNAL(downLoadDataChanged(QString)), SLOT(getChannelFinished()));
    m_getChannelThread->startToDownload(QString());
}

void TTKRadioHelper::getChannelFinished()
{
    ChannelInfos channels = m_getChannelThread->getMusicChannel();
    if(m_songsThread == nullptr || m_playListThread == nullptr)
    {
        m_playListThread = new MusicRadioPlayListThread(this, m_cookJar);
        connect(m_playListThread, SIGNAL(downLoadDataChanged(QString)), SLOT(getPlayListFinished()));
        m_songsThread = new MusicRadioSongsThread(this, m_cookJar);
        connect(m_songsThread, SIGNAL(downLoadDataChanged(QString)), SLOT(getSongInfoFinished()));
    }

    if(!channels.isEmpty() && m_playListThread)
    {
        m_playListThread->startToDownload(channels.first().m_id);
    }
}

void TTKRadioHelper::getPlayListFinished()
{
    m_playListIds = m_playListThread->getMusicPlayList();
    if(m_songsThread && !m_playListIds.isEmpty())
    {
        m_songsThread->startToDownload(m_playListIds.first());
    }
}

void TTKRadioHelper::getSongInfoFinished()
{
    SongRadioInfo info;
    if(m_songsThread)
    {
        info = m_songsThread->getMusicSongInfo();
    }

    if(info.m_songUrl.isEmpty())
    {
        return;
    }

//    m_player->setMedia(QUrl(info.m_songUrl));
//    m_player->play();
}
