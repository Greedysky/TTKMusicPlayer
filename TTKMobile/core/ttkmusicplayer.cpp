#include "ttkmusicplayer.h"

#include <QMediaPlayer>

TTKMusicPlayer::TTKMusicPlayer(QObject *parent)
    : QObject(parent)
{
    m_state = StoppedState;
    m_player = new  QMediaPlayer(this);
    connect(m_player, SIGNAL(positionChanged(qint64)), SIGNAL(positionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), SIGNAL(durationChanged(qint64)));
}

TTKMusicPlayer::~TTKMusicPlayer()
{
    delete m_player;
}

int TTKMusicPlayer::state() const
{
    return static_cast<int>(m_state);
}

qint64 TTKMusicPlayer::duration() const
{
    return m_player->duration();
}

qint64 TTKMusicPlayer::position() const
{
    return m_player->position();
}

void TTKMusicPlayer::setPosition(qint64 position)
{
    m_player->setPosition(position);
}

int TTKMusicPlayer::volume() const
{
    return m_player->volume();
}

void TTKMusicPlayer::setVolume(int volume)
{
    m_player->setVolume(volume);
}

void TTKMusicPlayer::setMedia(const QString &url)
{
    m_player->setMedia(QUrl(url));
}

void TTKMusicPlayer::play()
{
    m_state = PlayingState;
    m_player->play();
}

void TTKMusicPlayer::pause()
{
    m_state = PausedState;
    m_player->pause();
}

void TTKMusicPlayer::stop()
{
    m_state = StoppedState;
    m_player->stop();
}
