#include "musicplayer.h"
#include "musicplaylist.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"

#include <qmmp/soundcore.h>
#include <qmath.h>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
{
    m_playlist = nullptr;
    m_state = MusicObject::StoppedState;
    m_enhanced = EnhancedOff;
    m_core = new SoundCore(this);
    m_posOnCircle = 0;
    m_volumeMusic3D = 0;
    m_duration = 0;
    m_durationTimes = 0;

    setEnabledEffect(false);

    m_timer.setInterval(MT_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(update()));
    G_CONNECTION_PTR->setValue(className(), this);
}

MusicPlayer::~MusicPlayer()
{
    delete m_core;
}

bool MusicPlayer::isPlaying() const
{
    return m_state == MusicObject::PlayingState;
}

MusicObject::PlayState MusicPlayer::state() const
{
    return m_state;
}

void MusicPlayer::setPlaylist(MusicPlaylist *playlist)
{
    m_playlist = playlist;
}

MusicPlaylist *MusicPlayer::playlist() const
{
    return m_playlist;
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

void MusicPlayer::playNext()
{
    int index = m_playlist->currentIndex();
    m_playlist->setCurrentIndex((++index >= m_playlist->count()) ? 0 : index);
}

void MusicPlayer::playPrevious()
{
    int index = m_playlist->currentIndex();
    m_playlist->setCurrentIndex((--index < 0) ? 0 : index);
}

int MusicPlayer::volume() const
{
    return isMuted() ? 0 : m_core->volume();
}

void MusicPlayer::setVolume(int volume)
{
    m_volumeMusic3D = volume;
    m_core->setVolume(volume);
}

bool MusicPlayer::isMuted() const
{
    return m_core->isMuted();
}

void MusicPlayer::setMuted(bool muted)
{
    m_volumeMusic3D = muted ? 0 : m_core->volume();
    m_core->setMuted(muted);
}

void MusicPlayer::setMusicEnhanced(Enhanced type)
{
    m_enhanced = type;

    if(m_enhanced == Enhanced3D)
    {
        m_volumeMusic3D = volume();
    }
    else
    {
        m_core->setBalance(0);
        m_core->setVolume(m_volumeMusic3D, m_volumeMusic3D);

        switch(m_enhanced)
        {
            case EnhancedOff: setEqualizerEffect({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}); break;
            case EnhancedVocal: setEqualizerEffect({0, 0, 4, 1, -5, -1, 2, -2, -4, -4, 0}); break;
            case EnhancedNICAM: setEqualizerEffect({6, -12, -12, -9, -6, -3, -12, -9, -6, -3, -12}); break;
            case EnhancedSubwoofer: setEqualizerEffect({6, 6, -10, -10, 0, 0, -3, -5, -7, -9, -11}); break;
            default: break;
        }
    }
}

MusicPlayer::Enhanced MusicPlayer::musicEnhanced() const
{
    return m_enhanced;
}

void MusicPlayer::play()
{
    if(m_playlist->isEmpty())
    {
        m_state = MusicObject::StoppedState;
        return;
    }

    m_state = MusicObject::PlayingState;
    const Qmmp::State state = m_core->state(); ///Get the current state of play

    const QString &mediaPath = m_playlist->currentMediaPath();
    if(m_currentMedia == mediaPath && state == Qmmp::Paused)
    {
        m_core->pause(); ///When the pause time for recovery
        m_timer.start();
        update();
        return;
    }

    m_currentMedia = mediaPath;
    ///The current playback path
    if(!m_core->play(m_currentMedia))
    {
        m_state = MusicObject::StoppedState;
        return;
    }

    m_timer.start();

    m_durationTimes = 0;
    queryCurrentDuration();
    Q_EMIT positionChanged(0);
}

void MusicPlayer::pause()
{
    if(m_state != MusicObject::PausedState)
    {
        m_core->pause();
        m_timer.stop();
        m_state = MusicObject::PausedState;
    }
}

void MusicPlayer::stop()
{
    if(m_state != MusicObject::StoppedState)
    {
        m_core->stop();
        m_timer.stop();
        m_state = MusicObject::StoppedState;
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

void MusicPlayer::setEnabledEffect(bool enable)
{
    if(!enable)
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

    if(m_enhanced == Enhanced3D && !isMuted())
    {
        ///3D music settings
        setEnabledEffect(false);
        m_posOnCircle += 0.5f;
        m_core->setVolume(fabs(100 * cosf(m_posOnCircle)), fabs(100 * sinf(m_posOnCircle * 0.5f)));
    }

    const Qmmp::State state = m_core->state();
    if(!(state == Qmmp::Playing || state == Qmmp::Paused || state == Qmmp::Buffering))
    {
        m_timer.stop();
        if(m_playlist->playbackMode() == MusicObject::PlayOnce)
        {
            m_core->stop();
            Q_EMIT positionChanged(0);
            Q_EMIT stateChanged(MusicObject::StoppedState);
            return;
        }

        m_playlist->setCurrentIndex();
        if(m_playlist->playbackMode() == MusicObject::PlayOrder && m_playlist->currentIndex() == -1)
        {
            m_core->stop();
            Q_EMIT positionChanged(0);
            Q_EMIT stateChanged(MusicObject::StoppedState);
            return;
        }
        play();
    }
}

void MusicPlayer::queryCurrentDuration()
{
    const qint64 d = duration();
    if((d == 0 || m_duration == d) && m_durationTimes++ < 10)
    {
        QTimer::singleShot(50 * MT_MS, this, SLOT(queryCurrentDuration()));
    }
    else
    {
        Q_EMIT durationChanged(m_duration = d);
        Q_EMIT positionChanged(position());
    }
}
