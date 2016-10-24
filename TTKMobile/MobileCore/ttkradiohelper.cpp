#include "ttkradiohelper.h"
#include "musicradiochannelthread.h"
#include "musicradioplaylistthread.h"
#include "musicradiosongsthread.h"
#include "musicdatadownloadthread.h"
#include "musictime.h"

#include <QMediaPlayer>
#include <QNetworkCookieJar>

TTKRadioHelper::TTKRadioHelper(QObject *parent)
    : QObject(parent)
{
    MusicTime::timeSRand();
    m_cookJar = new QNetworkCookieJar(this);
    m_getChannelThread = nullptr;
    m_playListThread = nullptr;
    m_songsThread = nullptr;
    m_player = new QMediaPlayer(this);
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(playStateChanged()));
}

TTKRadioHelper::~TTKRadioHelper()
{
    delete m_getChannelThread;
    delete m_playListThread;
    delete m_songsThread;
    delete m_cookJar;
    delete m_player;
}

void TTKRadioHelper::init()
{
    if(!m_getChannelThread)
    {
        m_getChannelThread = new MusicRadioChannelThread(this, m_cookJar);
        connect(m_getChannelThread, SIGNAL(downLoadDataChanged(QString)), SLOT(getChannelFinished()));
        m_getChannelThread->startToDownload(QString());
    }
}

bool TTKRadioHelper::isPlaying() const
{
    return m_player->state() == QMediaPlayer::PlayingState;
}

void TTKRadioHelper::play()
{
    m_player->play();
}

void TTKRadioHelper::pause()
{
    m_player->pause();
}

void TTKRadioHelper::playStateChanged()
{
    if(m_player->state() == QMediaPlayer::StoppedState)
    {
        getPlayListFinished();
    }
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
        int index = qrand() % m_playListIds.count();
        m_songsThread->startToDownload( m_playListIds[index] );
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

    m_player->setMedia(QUrl(info.m_songUrl));
    m_player->play();

    QString name = ART_DIR_FULL + info.m_artistName + SKN_FILE;
    if(!QFile::exists(name))
    {
        MusicDataDownloadThread *picDwonload = new MusicDataDownloadThread(info.m_songPicUrl, name,
                                 MusicDownLoadThreadAbstract::Download_SmlBG, this);
        connect(picDwonload, SIGNAL(downLoadDataChanged(QString)), SLOT(picDownloadStateChanged()));
        picDwonload->startToDownload();
    }
    else
    {
        picDownloadStateChanged();
    }
}

void TTKRadioHelper::picDownloadStateChanged()
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

    emit picDownloadFinished( "file:///" + ART_DIR_FULL + info.m_artistName + SKN_FILE );
}
