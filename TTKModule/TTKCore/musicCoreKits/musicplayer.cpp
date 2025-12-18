#include "musicplayer.h"
#include "musicplaylist.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicsonghelper.h"

#include <cmath>
#include <qmmp/soundcore.h>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent),
      m_playlist(nullptr),
      m_state(TTK::PlayState::Stopped),
      m_duration(0),
      m_durationTimes(0),
      m_enhance()
{
    m_core = new SoundCore(this);
    setEnabledEffect(false);

    m_timer.setInterval(TTK_DN_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(update()));

    TTK_CONNECTIONPOOL_REGISTER(this);
}

MusicPlayer::~MusicPlayer()
{
    m_core->stop();
    m_timer.stop();
    delete m_core;
}

bool MusicPlayer::isPlaying() const noexcept
{
    return m_state == TTK::PlayState::Playing;
}

TTK::PlayState MusicPlayer::state() const noexcept
{
    return m_state;
}

void MusicPlayer::setPlaylist(MusicPlaylist *playlist) noexcept
{
    m_playlist = playlist;
}

qint64 MusicPlayer::duration() const
{
    return m_core->duration();
}

qint64 MusicPlayer::position() const
{
    return m_core->elapsed();
}

void MusicPlayer::setPosition(qint64 position)
{
    m_core->seek(position);
}

int MusicPlayer::volume() const
{
    return isMuted() ? 0 : m_core->volume();
}

void MusicPlayer::setVolume(int volume)
{
    m_enhance.m_volume3D = volume;
    m_core->setVolume(volume);
}

bool MusicPlayer::isMuted() const noexcept
{
    return m_core->isMuted();
}

void MusicPlayer::setMuted(bool muted)
{
    m_enhance.m_volume3D = muted ? 0 : m_core->volume();
    m_core->setMuted(muted);
}

void MusicPlayer::setEnhance(TTK::Enhance type)
{
    m_enhance.m_type = type;

    if(m_enhance.m_type == TTK::Enhance::M3D)
    {
        m_enhance.m_volume3D = volume();
    }
    else
    {
        m_core->setBalance(0);
        m_core->setVolumePerChannel(m_enhance.m_volume3D, m_enhance.m_volume3D);

        switch(m_enhance.m_type)
        {
            case TTK::Enhance::Off: setEqualizerEffect({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}); break;
            case TTK::Enhance::Vocal: setEqualizerEffect({0, 0, 4, 1, -5, -1, 2, -2, -4, -4, 0}); break;
            case TTK::Enhance::NICAM: setEqualizerEffect({6, -12, -12, -9, -6, -3, -12, -9, -6, -3, -12}); break;
            case TTK::Enhance::Subwoofer: setEqualizerEffect({6, 6, -10, -10, 0, 0, -3, -5, -7, -9, -11}); break;
            default: break;
        }
    }
}

TTK::Enhance MusicPlayer::enhance() const noexcept
{
    return m_enhance.m_type;
}

void MusicPlayer::play()
{
    if(m_playlist->isEmpty())
    {
        setCurrentPlayState(TTK::PlayState::Stopped);
        return;
    }

    setCurrentPlayState(TTK::PlayState::Playing);
    ///Get the current state of play
    const Qmmp::State state = m_core->state();
    const MusicPlayItem &item = m_playlist->currentItem();

    if((item.m_playlistRow != MUSIC_NETWORK_LIST) ?
       (item.m_path == m_currentMedia) :
       (TTK::generateNetworkSongPath(m_currentMedia) == TTK::generateNetworkSongPath(item.m_path)))
    {
        if(state == Qmmp::Paused)
        {
            ///When the pause time for recovery
            m_core->pause();
            update();
            return;
        }
        else if(state == Qmmp::Stopped)
        {
            return;
        }
    }

    m_currentMedia = (item.m_playlistRow != MUSIC_NETWORK_LIST) ? item.m_path : TTK::generateNetworkSongPath(item.m_path);
    ///The current playback path
    if(!m_core->play(m_currentMedia))
    {
        setCurrentPlayState(TTK::PlayState::Stopped);
        return;
    }

    m_timer.start();
    m_durationTimes = 0;

    generateDuration();
    Q_EMIT positionChanged(0);
}

void MusicPlayer::pause()
{
    if(m_state != TTK::PlayState::Paused)
    {
        m_core->pause();
        setCurrentPlayState(TTK::PlayState::Paused);
    }
}

void MusicPlayer::stop()
{
    if(m_state != TTK::PlayState::Stopped)
    {
        m_core->stop();
        m_timer.stop();
        m_currentMedia.clear();
        setCurrentPlayState(TTK::PlayState::Stopped);
    }
}

void MusicPlayer::setEqualizerEffect(const TTKIntList &hz)
{
    if(hz.count() != 11)
    {
        return;
    }

    EqSettings eq = m_core->eqSettings();
    eq.setPreamp(15 + hz[0]);
    eq.setEnabled(true);

    for(int i = 0; i < EqSettings::EQ_BANDS_10; ++i)
    {
        eq.setGain(i, hz[i + 1]);
    }
    m_core->setEqSettings(eq);
}

void MusicPlayer::setEnabledEffect(bool enabled)
{
    if(!enabled)
    {
        setEqualizerEffect({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    }
}

void MusicPlayer::setEqualizerConfig()
{
    ///Read the equalizer parameters from a configuration file
    if(G_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt())
    {
        setEnabledEffect(true);
        const QStringList &eqValue = G_SETTING_PTR->value(MusicSettingManager::EqualizerValue).toString().split(",");
        if(eqValue.count() == 11)
        {
            setEqualizerEffect({eqValue[0].toInt(), eqValue[1].toInt(), eqValue[2].toInt(), eqValue[3].toInt(),
                                eqValue[4].toInt(), eqValue[5].toInt(), eqValue[6].toInt(), eqValue[7].toInt(),
                                eqValue[8].toInt(), eqValue[9].toInt(), eqValue[10].toInt()});
        }
    }
    else
    {
        setEnabledEffect(false);
    }
}

void MusicPlayer::update()
{
    Q_EMIT positionChanged(position());

    if(m_enhance.m_type == TTK::Enhance::M3D && !isMuted())
    {
        ///3D music settings
        setEnabledEffect(false);
        m_enhance.m_circlePos += 0.5f;
        m_core->setVolumePerChannel(std::fabs(TTK_RN_MAX * std::cos(m_enhance.m_circlePos)), std::fabs(TTK_RN_MAX * std::sin(m_enhance.m_circlePos * 0.5f)));
    }

    const Qmmp::State state = m_core->state();
    if(state == Qmmp::NormalError || state == Qmmp::FatalError)
    {
        m_timer.stop();
        setStopState();
    }
    else if(state == Qmmp::Stopped)
    {
        if(m_state == TTK::PlayState::Paused)
        {
            return;
        }

        m_timer.stop();
        m_currentMedia.clear();

        if(m_playlist->playbackMode() == TTK::PlayMode::Once)
        {
            setStopState();
            return;
        }

        m_playlist->setCurrentIndex(TTK_LOW_LEVEL);
        if(m_playlist->playbackMode() == TTK::PlayMode::Order && m_playlist->currentIndex() == -1)
        {
            setStopState();
            return;
        }

        play();
    }
}

void MusicPlayer::generateDuration()
{
    const qint64 d = duration();
    if((d == 0 || m_duration == d) && m_durationTimes++ < 10)
    {
        TTK_SIGNLE_SHOT(generateDuration, TTK_SLOT);
    }
    else
    {
        m_duration = d;

        if(m_durationTimes >= 10)
        {
            const MusicPlayItem &item = m_playlist->currentItem();
            if(item.m_playlistRow == MUSIC_NETWORK_LIST)
            {
                m_duration = TTK::generateNetworkSongTime(item.m_path, false).toLongLong();
            }
        }

        Q_EMIT durationChanged(m_duration);
        Q_EMIT positionChanged(position());
    }
}

void MusicPlayer::setStopState()
{
    m_core->stop();
    Q_EMIT positionChanged(0);
    setCurrentPlayState(TTK::PlayState::Stopped);
}

void MusicPlayer::setCurrentPlayState(TTK::PlayState state) noexcept
{
    m_state = state;
    Q_EMIT stateChanged(m_state);
}
