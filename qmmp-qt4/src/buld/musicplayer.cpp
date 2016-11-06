#include "musicplayer.h"
#include "musicplaylist.h"
#include "../qmmp/soundcore.h"

#include <qmath.h>
#include <QDebug>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
{
    m_playlist = 0;
    m_music = 0;
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
    return m_music->totalTime();
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
//    m_music->EnableEcho(false);
//    m_music->EnableEqualizer(false);
//    if(m_musicEnhanced != Music3D && m_musicEnhanced != EnhancedOff)
//    {
//        m_music->EnableEqualizer(true);
//        setMusicEnhancedCase();
//    }
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

#ifdef Q_OS_WIN
void MusicPlayer::setSpectrum(HWND wnd, int w, int h, int x, int y)
{
//    if(!m_music)
//    {
//        return;
//    }
//    /// set graph type to AREA, left channel on top
//    m_music->SetFFTGraphParam(gpGraphType, gtAreaLeftOnTop);
//    /// set linear scale
//    m_music->SetFFTGraphParam(gpHorizontalScale, gsLinear);
//    m_music->DrawFFTGraphOnHWND(wnd, x, y, w, h);
}
#endif

void MusicPlayer::play()
{
    if(m_playlist->isEmpty())
    {
        m_state = StoppedState;
        return;
    }

    m_state = PlayingState;
    Qmmp::State state = m_music->state(); ///Get the current state of play
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
//        M_LOGGER << "Error1." << m_music->GetError() << LOG_END;
        return;
    }

    m_timer.start(1000);
    ///Every second emits a signal change information
    emit positionChanged(0);
    emit durationChanged( duration() );

//    if(m_music->Play() == 0)
//    {
//        M_LOGGER << "Error2." << m_music->GetError() << LOG_END;
//        return;
//    }

    ////////////////////////////////////////////////
    ///Read the configuration settings for the sound
//    int volumn = M_SETTING->value(MusicSettingManager::VolumeChoiced).toInt();
//    if(volumn != -1)
//    {
//        setVolume(volumn);
//    }
    ////////////////////////////////////////////////
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
    emit positionChanged( position() );

    if(m_musicEnhanced == Music3D)
    {   ///3D music settings
//        m_music->EnableEcho(true);
//        m_posOnCircle += 0.5f;
//        TEchoEffect effect;
//        effect.nLeftDelay = 450;
//        effect.nLeftEchoVolume = 20;
//        effect.nLeftSrcVolume = 100 * cosf(m_posOnCircle);
//        effect.nRightDelay = 500;
//        effect.nRightEchoVolume = 20;
//        effect.nRightSrcVolume = 100 * sinf(m_posOnCircle * 0.5f);
//        m_music->SetEchoParam(&effect, 1);
    }

    Qmmp::State state = m_music->state();
    if(state != Qmmp::Playing && state != Qmmp::Paused)
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
//    switch(m_musicEnhanced)
//    {
//        case MusicVocal:
//            setEqEffect(MIntList()<<  0<<  4<<  1<< -5<< -1<<  2<< -2<< -4<< -4<<  0);
//            break;
//        case MusicNICAM:
//            setEqEffect(MIntList()<<-12<<-12<< -9<< -6<< -3<<-12<< -9<< -6<< -3<<-12);
//            break;
//        case MusicSubwoofer:
//            setEqEffect(MIntList()<<  6<<-10<<-10<<  0<<  0<< -3<< -5<< -7<< -9<<-11);
//            break;
//        default:
//            break;
//    }
}

void MusicPlayer::removeCurrentMedia()
{
    if(m_music)
    {
        m_timer.stop();
        m_music->stop();
    }
}

//void MusicPlayer::setEqEffect(const MIntList &hz)
//{
//    m_equalizer->setEqEffect(hz);
//}

//void MusicPlayer::setEnaleEffect(bool enable)
//{
//    m_equalizer->setEnaleEffect(enable);
//}

//void MusicPlayer::setEqInformation()
//{
//    m_equalizer->readEqInformation();
//}

