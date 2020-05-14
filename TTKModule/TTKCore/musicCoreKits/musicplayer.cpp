#include "musicplayer.h"
#include "musicplaylist.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
///qmmp incldue
#include "soundcore.h"
///
#include <qmath.h>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
{
    m_playlist = nullptr;
    m_state = MusicObject::PS_StoppedState;
    m_musicEnhanced = EnhancedOff;
    m_music = new SoundCore(this);
    m_posOnCircle = 0;
    m_volumeMusic3D = 0;
    m_duration = 0;
    m_durationTimes = 0;

    setEnabledEffect(false);

    connect(&m_timer, SIGNAL(timeout()), SLOT(update()));
    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicPlayer::~MusicPlayer()
{
    delete m_music;
}

bool MusicPlayer::isPlaying() const
{
    return m_state == MusicObject::PS_PlayingState;
}

MusicObject::PlayState MusicPlayer::state() const
{
    return m_state;
}

void MusicPlayer::setPlaylist(MusicPlaylist *playlist)
{
    delete m_playlist;
    m_playlist = playlist;
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

void MusicPlayer::setPosition(qint64 position)
{
    m_music->seek(position);
}

void MusicPlayer::playNext()
{
    int index = m_playlist->currentIndex();
    m_playlist->setCurrentIndex((++index >= m_playlist->mediaCount()) ? 0 : index);
}

void MusicPlayer::playPrevious()
{
    int index = m_playlist->currentIndex();
    m_playlist->setCurrentIndex((--index < 0) ? 0 : index);
}

int MusicPlayer::volume() const
{
    return isMuted() ? 0 : m_music->volume();
}

void MusicPlayer::setVolume(int volume)
{
    m_volumeMusic3D = volume;
    m_music->setVolume(volume);
}

bool MusicPlayer::isMuted() const
{
    return m_music->isMuted();
}

void MusicPlayer::setMuted(bool muted)
{
    m_volumeMusic3D = muted ? 0 : m_music->volume();
    m_music->setMuted(muted);
}

void MusicPlayer::setMusicEnhanced(Enhanced type)
{
    m_musicEnhanced = type;

    if(m_musicEnhanced == Enhanced3D)
    {
        m_volumeMusic3D = volume();
    }
    else
    {
        m_music->setBalance(0);
        m_music->setVolume(m_volumeMusic3D, m_volumeMusic3D);
        setMusicEnhancedCase();
    }
}

MusicPlayer::Enhanced MusicPlayer::getMusicEnhanced() const
{
    return m_musicEnhanced;
}

void MusicPlayer::play()
{
    if(m_playlist->isEmpty())
    {
        m_state = MusicObject::PS_StoppedState;
        return;
    }

    m_state = MusicObject::PS_PlayingState;
    const Qmmp::State state = m_music->state(); ///Get the current state of play

    if(m_currentMedia == m_playlist->currentMediaPath() && state == Qmmp::Paused)
    {
        m_music->pause(); ///When the pause time for recovery
        m_timer.start(MT_S2MS);
        return;
    }

    m_currentMedia = m_playlist->currentMediaPath();
    ///The current playback path
    if(!m_music->play(m_currentMedia))
    {
        m_state = MusicObject::PS_StoppedState;
        return;
    }

    m_durationTimes = 0;
    m_timer.start(MT_S2MS);

    queryCurrentDuration();
    ///Every second Q_EMITs a signal change information
    Q_EMIT positionChanged(0);
}

void MusicPlayer::pause()
{
    m_music->pause();
    m_timer.stop();
    m_state = MusicObject::PS_PausedState;
}

void MusicPlayer::stop()
{
    m_music->stop();
    m_timer.stop();
    m_state = MusicObject::PS_StoppedState;
}

void MusicPlayer::setEqEffect(const TTKIntList &hz)
{
    if(hz.count() != 11)
    {
        return;
    }

    EqSettings eq = m_music->eqSettings();
    eq.setPreamp(15 + hz[0]);
    eq.setEnabled(true);
    for(int i=0; i<EqSettings::EQ_BANDS_10; ++i)
    {
        eq.setGain(i, hz[i + 1]);
    }
    m_music->setEqSettings(eq);
}

void MusicPlayer::setEnabledEffect(bool enable)
{
    if(!enable)
    {
        setEqEffect(TTKIntList()<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0);
    }
}

void MusicPlayer::setEqInformation()
{
    ///Read the equalizer parameters from a configuration file
    if(M_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt())
    {
        setEnabledEffect(true);
        const QStringList &eqValue = M_SETTING_PTR->value(MusicSettingManager::EqualizerValue).toString().split(",");
        if(eqValue.count() == 11)
        {
            TTKIntList hz;
            hz << eqValue[0].toInt() << eqValue[1].toInt() << eqValue[2].toInt()
               << eqValue[3].toInt() << eqValue[4].toInt() << eqValue[5].toInt()
               << eqValue[6].toInt() << eqValue[7].toInt() << eqValue[8].toInt()
               << eqValue[9].toInt() << eqValue[10].toInt();
            setEqEffect(hz);
        }
    }
    else
    {
        setEnabledEffect(false);
    }
}

void MusicPlayer::removeCurrentMedia()
{
    m_timer.stop();
    m_music->stop();
}

void MusicPlayer::update()
{
    Q_EMIT positionChanged(position());

    if(m_musicEnhanced == Enhanced3D && !isMuted())
    {
        ///3D music settings
        setEnabledEffect(false);
        m_posOnCircle += 0.5f;
        m_music->setVolume(fabs(100 * cosf(m_posOnCircle)), fabs(100 * sinf(m_posOnCircle * 0.5f)));
    }

    const Qmmp::State state = m_music->state();
    if(state != Qmmp::Playing && state != Qmmp::Paused)
    {
        m_timer.stop();
        if(m_playlist->playbackMode() == MusicObject::PM_PlayOnce)
        {
            m_music->stop();
            Q_EMIT positionChanged(0);
            Q_EMIT stateChanged(MusicObject::PS_StoppedState);
            return;
        }

        m_playlist->setCurrentIndex();
        if(m_playlist->playbackMode() == MusicObject::PM_PlayOrder && m_playlist->currentIndex() == -1)
        {
            m_music->stop();
            Q_EMIT positionChanged(0);
            Q_EMIT stateChanged(MusicObject::PS_StoppedState);
            return;
        }
        play();
    }
}

void MusicPlayer::queryCurrentDuration()
{
    const qint64 dur = duration();
    if((dur == 0 || m_duration == dur) && m_durationTimes++ < 10)
    {
        QTimer::singleShot(50 * MT_MS, this, SLOT(queryCurrentDuration()));
    }
    else
    {
        Q_EMIT durationChanged(m_duration = dur);
    }
}

void MusicPlayer::setMusicEnhancedCase()
{
    switch(m_musicEnhanced)
    {
        case EnhancedOff:
            setEqEffect(TTKIntList()<<  0<<  0<<  0<<  0<<  0<<  0<<  0<<  0<<  0<<  0<<  0);
            break;
        case EnhancedVocal:
            setEqEffect(TTKIntList()<<  0<<  0<<  4<<  1<< -5<< -1<<  2<< -2<< -4<< -4<<  0);
            break;
        case EnhancedNICAM:
            setEqEffect(TTKIntList()<<  6<<-12<<-12<< -9<< -6<< -3<<-12<< -9<< -6<< -3<<-12);
            break;
        case EnhancedSubwoofer:
            setEqEffect(TTKIntList()<<  6<<  6<<-10<<-10<<  0<<  0<< -3<< -5<< -7<< -9<<-11);
            break;
        default:
            break;
    }
}
