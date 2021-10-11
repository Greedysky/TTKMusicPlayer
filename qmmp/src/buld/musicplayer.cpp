#include "musicplayer.h"
#include "musicplaylist.h"
#include "../qmmp/soundcore.h"

#include <math.h>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
{
    m_playlist = nullptr;
    m_music = nullptr;
    m_state = StoppedState;
    m_musicEnhanced = EnhancedOff;
    m_music = new SoundCore(this);
    m_posOnCircle = 0;

    connect(&m_timer, SIGNAL(timeout()), SLOT(setTimeOut()));
}

MusicPlayer::~MusicPlayer()
{
    delete m_music;
}

MusicPlayer::State MusicPlayer::state() const
{
    return m_state;
}

MusicPlaylist *MusicPlayer::playlist() const
{
    return m_playlist;
}

qint64 MusicPlayer::duration() const
{
    return m_music->duration();
}

qint64 MusicPlayer::position() const
{
    return m_music->elapsed();
}

int MusicPlayer::volume() const
{
    return m_music->volume();
}

bool MusicPlayer::isMuted() const
{
    return m_music->isMuted();
}

void MusicPlayer::setMusicEnhanced(Enhanced type)
{
    m_musicEnhanced = type;
}

MusicPlayer::Enhanced MusicPlayer::getMusicEnhanced() const
{
    return m_musicEnhanced;
}

QStringList MusicPlayer::supportFormatsString()
{
    return QStringList()<< "mp3" << "mp2" << "mp1" << "wav" << "ogg"
                        << "flac" << "ac3" << "aac" << "oga" << "pcm";
}

QStringList MusicPlayer::supportFormatsFilterString()
{
    return QStringList()<< "*.mp3" << "*.mp2" << "*.mp1" << "*.wav"
                        << "*.ogg" << "*.flac" << "*.ac3" << "*.aac"
                        << "*.oga" << "*.pcm";
}

QStringList MusicPlayer::supportFormatsFilterDialogString()
{
    return QStringList()<< "Mp3 File(*.mp3)" << "Mp2 File(*.mp2)" << "Mp1 File(*.mp1)"
                        << "Wav File(*.wav)" << "Ogg File(*.ogg)" << "Flac File(*.flac)"
                        << "Ac3 File(*.ac3)" << "Aac File(*.aac)" << "Oga File(*.oga)"
                        << "Pcm File(*.pcm)";
}

void MusicPlayer::play()
{
    if(m_playlist->isEmpty())
    {
        m_state = StoppedState;
        return;
    }

    m_state = PlayingState;
    Qmmp::State state = m_music->state();
    if(m_currentMedia == m_playlist->currentMediaString() && state == Qmmp::Paused)
    {
        m_music->pause(); ///When the pause time for recovery
        m_timer.start(1000);
        return;
    }

    m_currentMedia = m_playlist->currentMediaString();
    ///The current playback path
    if(!m_music->play(m_currentMedia))
    {
        m_state = StoppedState;
        return;
    }

    m_timer.start(1000);
    ///Every second emits a signal change information
    emit positionChanged(0);
    emit durationChanged(duration());
}

void MusicPlayer::playNext()
{
    int index = m_playlist->currentIndex();
    m_playlist->setCurrentIndex((++index >= m_playlist->mediaCount()) ? 0 : index);
}

void MusicPlayer::playPrivious()
{
    int index = m_playlist->currentIndex();
    m_playlist->setCurrentIndex((--index < 0) ? 0 : index );
}

void MusicPlayer::pause()
{
    m_music->pause();
    m_timer.stop();
    m_state = PausedState;
}

void MusicPlayer::stop()
{
    m_music->stop();
    m_timer.stop();
    m_state = StoppedState;
}

void MusicPlayer::setPosition(qint64 position)
{
    m_music->seek(position);
}

void MusicPlayer::setVolume(int volume)
{
    m_music->setVolume(volume);
}

void MusicPlayer::setMuted(bool muted)
{
    m_music->setMuted(muted);
}

void MusicPlayer::setPlaylist(MusicPlaylist *playlist)
{
    m_playlist = playlist;
    connect(m_playlist, SIGNAL(removeCurrentMedia()), SLOT(removeCurrentMedia()));
}

void MusicPlayer::setTimeOut()
{
    emit positionChanged(position());

    Qmmp::State state = m_music->state();
    if(!(state == Qmmp::Playing || state == Qmmp::Paused || state == Qmmp::Buffering))
    {
        m_timer.stop();
        if(m_playlist->playbackMode() == MC_PlayOnce)
        {
            m_music->stop();
            emit positionChanged(0);
            emit stateChanged(StoppedState);
            return;
        }
        m_playlist->setCurrentIndex();
        if(m_playlist->playbackMode() == MC_PlayOrder &&
           m_playlist->currentIndex() == -1)
        {
            m_music->stop();
            emit positionChanged(0);
            emit stateChanged(StoppedState);
            return;
        }
        play();
    }
}

void MusicPlayer::setMusicEnhancedCase()
{
}

void MusicPlayer::removeCurrentMedia()
{
    if(m_music)
    {
        m_timer.stop();
        m_music->stop();
    }
}
