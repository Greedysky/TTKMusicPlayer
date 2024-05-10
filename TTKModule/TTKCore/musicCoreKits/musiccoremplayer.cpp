#include "musiccoremplayer.h"
#include "musicabstractnetwork.h"
#include "miniprocess.h"

#include <QProcess>

MusicCoreMPlayer::MusicCoreMPlayer(QObject *parent)
    : QObject(parent),
      m_process(nullptr),
      m_playState(TTK::PlayState::Stopped),
      m_category(Module::Null)
{
    m_timer.setInterval(TTK_DN_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));

    m_checkTimer.setInterval(10 * TTK_DN_S2MS);
    connect(&m_checkTimer, SIGNAL(timeout()), SLOT(checkTimerout()));
}

MusicCoreMPlayer::~MusicCoreMPlayer()
{
    closeModule();
}

void MusicCoreMPlayer::setMedia(Module type, const QString &url, int winId)
{
    closeModule();
    if(!QFile::exists(MAKE_PLAYER_PATH_FULL))
    {
        TTK_ERROR_STREAM("Lack of plugin file");
        return;
    }

    m_category = type;
    m_playState = TTK::PlayState::Stopped;
    m_process = new QProcess(this);
    connect(m_process, SIGNAL(finished(int)), SIGNAL(finished(int)));

    switch(m_category)
    {
        case Module::Radio: setRadioMedia(url); break;
        case Module::Music: setMusicMedia(url); break;
        case Module::Video: setVideoMedia(url, winId); break;
        case Module::Null: break;
        default: break;
    }
}

void MusicCoreMPlayer::closeModule()
{
    m_timer.stop();
    m_checkTimer.stop();

    if(m_process)
    {
        m_process->kill();
        delete m_process;
        m_process = nullptr;
        TTK::killProcessByName(MAKE_PLAYER_NAME);
    }
}

void MusicCoreMPlayer::setRadioMedia(const QString &url)
{
    Q_EMIT mediaChanged(url);

    QStringList arguments;
    arguments << "-softvol" << "-slave" << "-quiet" << "-vo" << "directx:noaccel" << url;
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(dataRecieve()));
    m_process->start(MAKE_PLAYER_PATH_FULL, arguments);
}

void MusicCoreMPlayer::setMusicMedia(const QString &url)
{
    Q_EMIT mediaChanged(url);

    QStringList arguments;
    arguments << "-cache" << "5000" << "-softvol" << "-slave" << "-quiet" << "-vo" << "directx:noaccel" << url;
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(dataRecieve()));
    m_process->start(MAKE_PLAYER_PATH_FULL, arguments);
}

void MusicCoreMPlayer::setVideoMedia(const QString &url, int winId)
{
    Q_EMIT mediaChanged(url);

    QStringList arguments;
    arguments << "-cache" << "5000" << "-softvol" << "-slave" << "-quiet" << "-zoom" << "-wid" << QString::number(winId);
#ifdef Q_OS_WIN
    arguments << "-vo" << "direct3d";
#else
    arguments << "-vo" << "x11";
#endif
    arguments << url;

    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(durationRecieve()));
    m_process->start(MAKE_PLAYER_PATH_FULL, arguments);
}

void MusicCoreMPlayer::setPosition(qint64 pos)
{
    if(!m_process)
    {
        return;
    }

    m_process->write(QString("seek %1 2\n").arg(pos).toUtf8());
}

void MusicCoreMPlayer::setMuted(bool mute)
{
    if(!m_process)
    {
        return;
    }

    m_process->write(QString("mute %1\n").arg(mute ? 1 : 0).toUtf8());
}

void MusicCoreMPlayer::setVolume(int value)
{
    if(!m_process)
    {
        return;
    }

    m_process->write(QString("volume %1 1\n").arg(value).toUtf8());
}

bool MusicCoreMPlayer::isPlaying() const
{
    return m_playState == TTK::PlayState::Playing;
}

void MusicCoreMPlayer::play()
{
    m_timer.stop();
    m_checkTimer.start();

    if(!m_process)
    {
        return;
    }

    m_process->write("pause\n");
    if(m_playState == TTK::PlayState::Stopped || m_playState == TTK::PlayState::Paused)
    {
        m_playState = TTK::PlayState::Playing;
        connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(positionRecieve()));
        m_process->write("get_time_pos\n");
    }
    else
    {
        m_playState = TTK::PlayState::Paused;
        disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(positionRecieve()));
    }
}

void MusicCoreMPlayer::stop()
{
    m_playState = TTK::PlayState::Stopped;
    m_timer.stop();
    m_checkTimer.stop();

    if(!m_process)
    {
        return;
    }

    m_process->write("quit\n");
}

void MusicCoreMPlayer::durationRecieve()
{
    while(m_process->canReadLine())
    {
        QString message(m_process->readLine());
        if(message.startsWith("ANS_LENGTH"))
        {
            message.remove(TTK_WLINEFEED);
            disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(durationRecieve()));
            Q_EMIT durationChanged(message.mid(11).toFloat());
            return;
        }
    }
}

void MusicCoreMPlayer::dataRecieve()
{
    switch(m_category)
    {
        case Module::Radio: positionRecieve(); break;
        case Module::Music: standardRecieve(); break;
        case Module::Video: positionRecieve(); break;
        case Module::Null: break;
        default: break;
    }
}

void MusicCoreMPlayer::positionRecieve()
{
    m_timer.start();
    while(m_process->canReadLine())
    {
        QString message(m_process->readLine());
        if(message.startsWith("ANS_TIME_POSITION"))
        {
            message.remove(TTK_WLINEFEED);
            Q_EMIT positionChanged(message.mid(18).toFloat());
        }
    }
}

void MusicCoreMPlayer::standardRecieve()
{
    while(m_process->canReadLine())
    {
        QString message(m_process->readLine());
        if(message.startsWith("ANS_LENGTH"))
        {
            message.remove(TTK_WLINEFEED);
            Q_EMIT durationChanged(message.mid(11).toFloat());
        }

        if(message.startsWith("ANS_TIME_POSITION"))
        {
            message.remove(TTK_WLINEFEED);
            Q_EMIT positionChanged(message.mid(18).toFloat());
        }
    }
}

void MusicCoreMPlayer::timeout()
{
    m_process->write("get_time_length\n");
    m_process->write("get_time_pos\n");
}

void MusicCoreMPlayer::checkTimerout()
{
    if(m_process && m_process->state() == QProcess::NotRunning)
    {
        m_checkTimer.stop();
        Q_EMIT finished(TTK_LOW_LEVEL);
    }
}
