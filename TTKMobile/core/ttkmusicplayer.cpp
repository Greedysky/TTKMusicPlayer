#include "ttkmusicplayer.h"
#include "ttkmusicplaylist.h"
#include "musicnumberdefine.h"
#include "musicsettingmanager.h"
///qmmp incldue
#include "soundcore.h"

TTKMusicPlayer::TTKMusicPlayer(QObject *parent)
    : QObject(parent)
{
    m_state = StoppedState;

    m_playlist = nullptr;
    m_state = StoppedState;
    m_music = new SoundCore(this);
    m_duration = 0;
    m_tryTimes = 0;

    connect(&m_timer, SIGNAL(timeout()), SLOT(update()));
}

TTKMusicPlayer::~TTKMusicPlayer()
{
    delete m_music;
}

int TTKMusicPlayer::state() const
{
    return static_cast<int>(m_state);
}

void TTKMusicPlayer::setPlaylist(TTKMusicPlaylist *playlist)
{
    delete m_playlist;
    m_playlist = playlist;
}

TTKMusicPlaylist *TTKMusicPlayer::playlist() const
{
    return m_playlist;
}

qint64 TTKMusicPlayer::duration() const
{
    return m_music->totalTime();
}

qint64 TTKMusicPlayer::position() const
{
    return m_music->elapsed();
}

void TTKMusicPlayer::setPosition(qint64 position)
{
    m_music->seek(position);
}

int TTKMusicPlayer::volume() const
{
    return isMuted() ? 0 : m_music->volume();
}

void TTKMusicPlayer::setVolume(int volume)
{
    m_music->setVolume(volume);
}

bool TTKMusicPlayer::isMuted() const
{
    return m_music->isMuted();
}

void TTKMusicPlayer::setMuted(bool muted)
{
    m_music->setMuted(muted);
}

QStringList TTKMusicPlayer::supportFormatsString()
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
                        << "aac";
}

QStringList TTKMusicPlayer::supportFormatsFilterString()
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
                        << "*.aac";
}

QStringList TTKMusicPlayer::supportFormatsFilterDialogString()
{
    return QStringList()<< "All File(*.*)" << "MusePack File(*.mpc)" << "WavPack File(*.wv)" << "Ogg Vorbis File(*.ogg)"
                        << "PCM File(*.wav *.au *.snd *.aif *.aiff *.8svx *.sph *.sf *.voc *.w64)"
                        << "CUE File(*.cue)" << "MPEG File(*.mp1 *.mp2 *.mp3 *.wav)"
                        << "Game File(*.ay *.gms *.gym *.hes *.kss *.nsf *.nsfe *.sap *.spc *.vgm *.vgz)"
                        << "FLAC File(*.flac *.oga)" << "Monkey's Audio File(*.ape)"
                        << "Ogg Opus File(*.opus)" << "ADTS AAC File(*.aac)" << "MIDI File(*.mid)"
                        << "FFmpeg File(*.wma *.ape *.tta *.m4a *.ra *.shn *.vqf *.ac3)"
                        << "ModPlug File(*.mod *.s3m *.xm *.it *.669 *.amf *.ams *.dbm *.dmf *.dsm *.far,mdl *.med *.mtm "
                           "*.okt *.ptm *.stm *.ult *.umx *.mt2 *.psm *.mdz *.s3z *.xmz *.itz *.mdr *.s3r *.xmr *.itr "
                           "*.dgz *.s3gz *.xmgz *.itgz)";
}

void TTKMusicPlayer::play()
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
        m_timer.start(MT_S2MS);
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
    m_timer.start(MT_S2MS);
    ///Every second emits a signal change information
    emit positionChanged(0);
    getCurrentDuration();

    ////////////////////////////////////////////////
    ///Read the configuration settings for the sound
    int volume = M_SETTING_PTR->value(MusicSettingManager::VolumeChoiced).toInt();
    if(volume != -1)
    {
        setVolume(volume);
    }
}

void TTKMusicPlayer::pause()
{
    m_state = PausedState;
    m_music->pause();
}

void TTKMusicPlayer::stop()
{
    m_state = StoppedState;
    m_music->stop();
}

void TTKMusicPlayer::update()
{
    emit positionChanged( position() );

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

void TTKMusicPlayer::getCurrentDuration()
{
    qint64 dur = duration();
    if( (dur == 0 || m_duration == dur) && m_tryTimes++ < 10 )
    {
        QTimer::singleShot(50*MT_MS, this, SLOT(getCurrentDuration()));
    }
    else
    {
        emit durationChanged( m_duration = dur );
    }
}
