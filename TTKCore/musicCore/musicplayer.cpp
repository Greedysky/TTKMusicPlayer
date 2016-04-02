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
    m_playlist = 0;
    m_music = 0;
    m_state = StoppedState;
    m_musicEnhanced = EnhancedOff;
    m_music = new SoundCore(this);
    m_posOnCircle = 0;
    m_volumeMusic3D = 0;
    m_duration = 0;
    m_tryTimes = 0;

    setEnaleEffect(false);

    connect(&m_timer, SIGNAL(timeout()), SLOT(setTimeOut()));
    M_CONNECTION->setValue("MusicPlayer", this);
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

    if(m_musicEnhanced == Music3D)
    {
        m_volumeMusic3D = volume();
    }
    else
    {
        m_music->setVolume(m_volumeMusic3D, m_volumeMusic3D);
        setMusicEnhancedCase();
    }
}

MusicPlayer::Enhanced MusicPlayer::getMusicEnhanced() const
{
    return m_musicEnhanced;
}

QStringList MusicPlayer::supportFormatsString()
{
    return QStringList()<< "mpc" << "wv" << "ogg" << "wav" << "au" << "snd" << "aif" << "aiff" << "8svx"
                        << "sph" << "sf" << "voc" << "w64" << "cue" << "mp1" << "mp2" << "mp3" << "wav"
                        << "ay" << "gms" << "gym" << "hes" << "kss" << "nsf" << "nsfe" << "sap" << "spc"
                        << "vgm" << "vgz" << "flac" << "oga" << "ape" << "wma" << "ape" << "tta" << "m4a"
                        << "ra" << "shn" << "vqf" << "ac3" << "mod" << "s3m" << "xm" << "it" << "669"
                        << "amf" << "ams" << "dbm" << "dmf" << "dsm" << "far" << "mdl" << "med" << "mtm"
                        << "okt" << "ptm" << "stm" << "ult" << "umx" << "mt2" << "psm" << "mdz" << "s3z"
                        << "xmz" << "itz" << "mdr" << "s3r" << "xmr" << "itr" << "dgz" << "s3gz" << "xmgz"
                        << "itgz" << "opus" << "mid"
#ifndef Q_OS_WIN
                        << "sid" << "mus" << "str" << "prg" << "P00"
#endif
                        << "aac";
}

QStringList MusicPlayer::supportFormatsFilterString()
{
    return QStringList()<< "*.mpc" << "*.wv" << "*.ogg" << "*.wav" << "*.au" << "*.snd" << "*.aif" << "*.aiff" << "*.8svx"
                        << "*.sph" << "*.sf" << "*.voc" << "*.w64" << "*.cue" << "*.mp1" << "*.mp2" << "*.mp3" << "*.wav"
                        << "*.ay" << "*.gms" << "*.gym" << "*.hes" << "*.kss" << "*.nsf" << "*.nsfe" << "*.sap" << "*.spc"
                        << "*.vgm" << "*.vgz" << "*.flac" << "*.oga" << "*.ape" << "*.wma" << "*.ape" << "*.tta" << "*.m4a"
                        << "*.ra" << "*.shn" << "*.vqf" << "*.ac3" << "*.mod" << "*.s3m" << "*.xm" << "*.it" << "*.669"
                        << "*.amf" << "*.ams" << "*.dbm" << "*.dmf" << "*.dsm" << "*.far" << "*.mdl" << "*.med" << "*.mtm"
                        << "*.okt" << "*.ptm" << "*.stm" << "*.ult" << "*.umx" << "*.mt2" << "*.psm" << "*.mdz" << "*.s3z"
                        << "*.xmz" << "*.itz" << "*.mdr" << "*.s3r" << "*.xmr" << "*.itr" << "*.dgz" << "*.s3gz" << "*.xmgz"
                        << "*.itgz" << "*.opus" << "*.mid"
#ifndef Q_OS_WIN
                        << "*.sid" << "*.mus" << "*.str" << "*.prg" << "*.P00"
#endif
                        << "*.aac";
}

QStringList MusicPlayer::supportFormatsFilterDialogString()
{
    return QStringList()<< "All File(*.*)" << "MusePack File(*.mpc)" << "WavPack File(*.wv)" << "Ogg Vorbis File(*.ogg)"
                        << "PCM File(*.wav *.au *.snd *.aif *.aiff *.8svx *.sph *.sf *.voc *.w64)"
                        << "CUE File(*.cue)" << "MPEG File(*.mp1 *.mp2 *.mp3 *.wav)"
                        << "Game File(*.ay *.gms *.gym *.hes *.kss *.nsf *.nsfe *.sap *.spc *.vgm *.vgz)"
                        << "FLAC File(*.flac *.oga)" << "Monkey's Audio File(*.ape)"
                        << "Ogg Opus File(*.opus)" << "ADTS AAC File(*.aac)" << "MIDI File(*.mid)"
#ifndef Q_OS_WIN
                        << "SID File(*.sid *.mus *.str *.prg *.P00)"
#endif
                        << "FFmpeg File(*.wma *.ape *.tta *.m4a *.ra *.shn *.vqf *.ac3)"
                        << "ModPlug File(*.mod *.s3m *.xm *.it *.669 *.amf *.ams *.dbm *.dmf *.dsm *.far,mdl *.med *.mtm "
                           "*.okt *.ptm *.stm *.ult *.umx *.mt2 *.psm *.mdz *.s3z *.xmz *.itz *.mdr *.s3r *.xmr *.itr "
                           "*.dgz *.s3gz *.xmgz *.itgz)";
}

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
        return;
    }

    m_tryTimes = 0;
    m_timer.start(1000);
    ///Every second emits a signal change information
    emit positionChanged(0);
    getCurrentDuration();

    ////////////////////////////////////////////////
    ///Read the configuration settings for the sound
    int volume = M_SETTING->value(MusicSettingManager::VolumeChoiced).toInt();
    if(volume != -1)
    {
        setVolume(volume);
    }
    ////////////////////////////////////////////////
}

void MusicPlayer::playNext()
{
    int index = m_playlist->currentIndex();
    m_playlist->setCurrentIndex((++index >= m_playlist->mediaCount()) ? 0 : index);
}

void MusicPlayer::playPrevious()
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
    m_volumeMusic3D = volume;
    m_music->setVolume(volume);
}

void MusicPlayer::setMuted(bool muted)
{
    m_volumeMusic3D = muted ? 0 : m_music->volume();
    m_music->setMuted(muted);
}

void MusicPlayer::setPlaylist(MusicPlaylist *playlist)
{
    m_playlist = playlist;
}

void MusicPlayer::setTimeOut()
{
    emit positionChanged( position() );

    if(m_musicEnhanced == Music3D && !m_music->isMuted())
    {
        ///3D music settings
        setEnaleEffect(false);
        m_posOnCircle += 0.5f;
        m_music->setVolume(abs(100 * cosf(m_posOnCircle)), abs(100 * sinf(m_posOnCircle * 0.5f)));
    }

    Qmmp::State state = m_music->state();
    if(state != Qmmp::Playing && state != Qmmp::Paused)
    {
        m_timer.stop();
        if(m_playlist->playbackMode() == MusicObject::MC_PlayOnce)
        {
            m_music->stop();
            emit positionChanged(0);
            emit stateChanged(StoppedState);
            return;
        }
        m_playlist->setCurrentIndex();
        if(m_playlist->playbackMode() == MusicObject::MC_PlayOrder &&
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
    switch(m_musicEnhanced)
    {
        case EnhancedOff:
            setEqEffect(MIntList()<<  0<<  0<<  0<<  0<<  0<<  0<<  0<<  0<<  0<<  0<<  0);
            break;
        case MusicVocal:
            setEqEffect(MIntList()<<  0<<  0<<  4<<  1<< -5<< -1<<  2<< -2<< -4<< -4<<  0);
            break;
        case MusicNICAM:
            setEqEffect(MIntList()<<  6<<-12<<-12<< -9<< -6<< -3<<-12<< -9<< -6<< -3<<-12);
            break;
        case MusicSubwoofer:
            setEqEffect(MIntList()<<  6<<  6<<-10<<-10<<  0<<  0<< -3<< -5<< -7<< -9<<-11);
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
        m_music->stop();
    }
}

void MusicPlayer::getCurrentDuration()
{
    qint64 dur = duration();
    if( (dur == 0 || m_duration == dur) && m_tryTimes++ < 10 )
    {
        QTimer::singleShot(50, this, SLOT(getCurrentDuration()));
    }
    else
    {
        emit durationChanged( m_duration = dur );
    }
}

void MusicPlayer::setEqEffect(const MIntList &hz)
{
    if(hz.count() != 11)
    {
        return;
    }

    EqSettings eq = m_music->eqSettings();
    eq.setPreamp(hz[0]);
    eq.setEnabled(true);
    for(int i=0; i<EqSettings::EQ_BANDS_10; ++i)
    {
        eq.setGain(i, hz[i + 1]);
    }
    m_music->setEqSettings(eq);
}

void MusicPlayer::setEnaleEffect(bool enable)
{
    if(enable == false)
    {
        setEqEffect(MIntList()<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0);
    }
}

void MusicPlayer::setEqInformation()
{
    ///Read the equalizer parameters from a configuration file
    if(M_SETTING->value(MusicSettingManager::EqualizerEnableChoiced).toInt())
    {
        setEnaleEffect(true);
        QStringList eqValue = M_SETTING->value(MusicSettingManager::EqualizerValueChoiced).toString().split(',');
        if(eqValue.count() == 11)
        {
            MIntList hz;
            hz << eqValue[0].toInt() << eqValue[1].toInt() << eqValue[2].toInt()
               << eqValue[3].toInt() << eqValue[4].toInt() << eqValue[5].toInt()
               << eqValue[6].toInt() << eqValue[7].toInt() << eqValue[8].toInt()
               << eqValue[9].toInt() << eqValue[10].toInt();
            setEqEffect(hz);
        }
    }
}

