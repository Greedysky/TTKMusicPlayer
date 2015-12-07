#include "musicplayer.h"
#include "musicplaylist.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"

#include <qmath.h>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
{
    m_playlist = nullptr;
    m_music = nullptr;
    m_state = StoppedState;
    m_musicEnhanced = EnhancedOff;
    m_music = CreateZPlay();
    Q_ASSERT(m_music);
    m_equalizer = new MusicEqualizer(m_music);
    m_posOnCircle = 0;
    m_currentVolumn = 100;

    connect(&m_timer, SIGNAL(timeout()), SLOT(setTimeOut()));
    M_CONNECTION->setValue("MusicPlayer", this);
}

MusicPlayer::~MusicPlayer()
{
    delete m_equalizer;
    m_music->Close();
    m_music->Release();
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
    TStreamInfo pInfo;
    m_music->GetStreamInfo(&pInfo);
    return pInfo.Length.hms.hour * 3600000 + pInfo.Length.hms.minute * 60000 +
           pInfo.Length.hms.second * 1000 + pInfo.Length.hms.millisecond;
}

qint64 MusicPlayer::position() const
{
    TStreamTime pos;
    m_music->GetPosition(&pos);
    return pos.hms.hour * 3600000 + pos.hms.minute * 60000 +
           pos.hms.second * 1000 + pos.hms.millisecond;
}

int MusicPlayer::volume() const
{
    uint vol;
    m_music->GetMasterVolume(&vol, &vol);
    return vol;
}

bool MusicPlayer::isMuted() const
{
    return (volume() == 0) ? true : false;
}

void MusicPlayer::setMusicEnhanced(Enhanced type)
{
    m_musicEnhanced = type;
    m_music->EnableEcho(false);
    m_music->EnableEqualizer(false);
    if(m_musicEnhanced != Music3D && m_musicEnhanced != EnhancedOff)
    {
        m_music->EnableEqualizer(true);
        setMusicEnhancedCase();
    }
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
    if(!m_music)
    {
        return;
    }
    /// set graph type to AREA, left channel on top
    m_music->SetFFTGraphParam(gpGraphType, gtAreaLeftOnTop);
    /// set linear scale
    m_music->SetFFTGraphParam(gpHorizontalScale, gsLinear);
    m_music->DrawFFTGraphOnHWND(wnd, x, y, w, h);
}
#endif

void MusicPlayer::play()
{
    if(m_playlist->isEmpty())
    {
        return;
    }

    TStreamStatus status;
    m_music->GetStatus(&status);///Get the current state of play
    if(m_currentMedia == m_playlist->currentMediaString() && status.fPause)
    {
        m_music->Resume();///When the pause time for recovery
        m_timer.start(1000);
        return;
    }

    m_music->Close();///For the release of the last memory
    m_currentMedia = m_playlist->currentMediaString();
    ///The current playback path
    if(m_music->OpenFileW(m_currentMedia.toStdWString().c_str(), sfAutodetect) == 0)
    {
        M_LOGGER << "Error1." << m_music->GetError() << LOG_END;
        return;
    }

    m_timer.start(1000);
    ///Every second emits a signal change information
    emit positionChanged(0);
    emit durationChanged( duration() );
    m_state = PlayingState;
    if(m_music->Play() == 0)
    {
        M_LOGGER << "Error2." << m_music->GetError() << LOG_END;
        return;
    }

    ////////////////////////////////////////////////
    ///Read the configuration settings for the sound
    int volumn = M_SETTING->value(MusicSettingManager::VolumeChoiced).toInt();
    if(volumn != -1)
    {
        setVolume(volumn);
    }
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
    m_music->Pause();
    m_timer.stop();
    m_state = PausedState;
}

void MusicPlayer::stop()
{
    m_music->Stop();
    m_timer.stop();
    m_state = StoppedState;
}

void MusicPlayer::setPosition(qint64 position)
{
    TStreamTime pTime;
    pTime.ms = position;
    m_music->Seek(tfMillisecond, &pTime, smFromBeginning);
}

void MusicPlayer::setVolume(int volume)
{
    m_currentVolumn = volume;
    m_music->SetMasterVolume(m_currentVolumn, m_currentVolumn);
}

void MusicPlayer::setMuted(bool muted)
{
    muted ? m_music->SetMasterVolume(0, 0) :
            m_music->SetMasterVolume(m_currentVolumn, m_currentVolumn);
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
        m_music->EnableEcho(true);
        m_posOnCircle += 0.5f;
        TEchoEffect effect;
        effect.nLeftDelay = 450;
        effect.nLeftEchoVolume = 20;
        effect.nLeftSrcVolume = 100 * cosf(m_posOnCircle);
        effect.nRightDelay = 500;
        effect.nRightEchoVolume = 20;
        effect.nRightSrcVolume = 100 * sinf(m_posOnCircle * 0.5f);
        m_music->SetEchoParam(&effect, 1);
    }

    TStreamStatus status;
    m_music->GetStatus(&status);
    if(!status.fPlay && !status.fPause)
    {
        m_timer.stop();
        if(m_playlist->playbackMode() == MusicObject::MC_PlayOnce)
        {
            m_music->Stop();
            emit positionChanged(0);
            emit stateChanged(StoppedState);
            return;
        }
        m_playlist->setCurrentIndex();
        if(m_playlist->playbackMode() == MusicObject::MC_PlayOrder &&
           m_playlist->currentIndex() == -1)
        {
            m_music->Stop();
            emit positionChanged(0);
            emit stateChanged(StoppedState);
            return;
        }
        play();
    }
}

void MusicPlayer::setMusicEnhancedCase()
{
    switch(m_musicEnhanced)
    {
        case MusicVocal:
            setEqEffect(MIntList()<<  0<<  4<<  1<< -5<< -1<<  2<< -2<< -4<< -4<<  0);
            break;
        case MusicNICAM:
            setEqEffect(MIntList()<<-12<<-12<< -9<< -6<< -3<<-12<< -9<< -6<< -3<<-12);
            break;
        case MusicSubwoofer:
            setEqEffect(MIntList()<<  6<<-10<<-10<<  0<<  0<< -3<< -5<< -7<< -9<<-11);
            break;
        default:
            break;
    }
}

void MusicPlayer::removeCurrentMedia()
{
    if(m_music)
    {
        m_timer.stop();
        m_music->Close();
    }
}

void MusicPlayer::setEqEffect(const MIntList &hz)
{
    m_equalizer->setEqEffect(hz);
}

void MusicPlayer::setEnaleEffect(bool enable)
{
    m_equalizer->setEnaleEffect(enable);
}

void MusicPlayer::setEqInformation()
{
    m_equalizer->readEqInformation();
}

void MusicPlayer::setSpEqEffect(MusicObject::SpecialEQ eq)
{
    if(m_state != PlayingState)
    {
        return;
    }
    switch(eq)
    {
        case MusicObject::EQ_EchoEffect:
                m_equalizer->setEchoEffect();break;
        case MusicObject::EQ_MixChannelEffect:
                m_equalizer->setMixChannelEffect();break;
        case MusicObject::EQ_ReverseEffect:
                m_equalizer->setReverseEffect();break;
        case MusicObject::EQ_SideCutEffect:
                m_equalizer->setSideCutEffect();break;
        case MusicObject::EQ_CenterCutEffect:
                m_equalizer->setCenterCutEffect();break;
        case MusicObject::EQ_RateUpEffect:
                m_equalizer->setRateUpEffect();break;
        case MusicObject::EQ_RateDownEffect:
                m_equalizer->setRateDownEffect();break;
        case MusicObject::EQ_PitchUpEffect:
                m_equalizer->setPitchUpEffect();break;
        case MusicObject::EQ_PitchDownEffect:
                m_equalizer->setPitchDownEffect();break;
        case MusicObject::EQ_TempoUpEffect:
                m_equalizer->setTempoUpEffect();break;
        case MusicObject::EQ_TempoDownEffect:
                m_equalizer->setTempoDownEffect();break;
        case MusicObject::EQ_FadeOutEffect:
                m_equalizer->setFadeOutEffect();break;
        case MusicObject::EQ_FadeInEffect:
                m_equalizer->setFadeInEffect();break;
    }
}
