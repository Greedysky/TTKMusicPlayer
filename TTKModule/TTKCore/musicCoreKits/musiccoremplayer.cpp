#include "musiccoremplayer.h"
#include "musicobject.h"
#include "musicnumberdefine.h"

#include <QProcess>

MusicCoreMPlayer::MusicCoreMPlayer(QObject *parent)
    : QObject(parent)
{
    m_process = nullptr;
    m_playState = MusicObject::PS_StoppedState;
    m_category = NullCategory;

    m_timer.setInterval(MT_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));

    m_checkTimer.setInterval(5*MT_S2MS);
    connect(&m_checkTimer, SIGNAL(timeout()), SLOT(checkTimerout()));
}

MusicCoreMPlayer::~MusicCoreMPlayer()
{
    m_timer.stop();
    m_checkTimer.stop();

    if(m_process)
    {
        m_process->kill();
    }

    delete m_process;
}

void MusicCoreMPlayer::setMedia(Category type, const QString &data, int winId)
{
    m_timer.stop();
    if(m_process)
    {
        m_process->kill();
        delete m_process;
        m_process = nullptr;
    }

    if(!QFile::exists(MAKE_PLAYER_FULL))
    {
        M_LOGGER_ERROR(tr("Lack of plugin file!"));
        return;
    }

    m_category = type;
    m_playState = MusicObject::PS_StoppedState;
    m_process = new QProcess(this);
    connect(m_process, SIGNAL(finished(int)), SIGNAL(finished(int)));

    switch(m_category)
    {
        case MusicCategory: setMusicMedia(data); break;
        case VideoCategory: setVideoMedia(data, winId); break;
        case NullCategory: break;
        default: break;
    }
}

void MusicCoreMPlayer::setVideoMedia(const QString &data, int winId)
{
    QStringList arguments;
    arguments << "-slave" << "-quiet" << "-wid";
    arguments << QString::number(winId);
#ifdef Q_OS_WIN
    arguments << "-vo" << "direct3d" << data;
#else
    arguments << "-vo" << "x11" << data;
#endif
    emit mediaChanged(data);

    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(durationRecieve()));
    m_process->write("get_time_length\n");
    m_process->start(MAKE_PLAYER_FULL, arguments);
}

void MusicCoreMPlayer::setMusicMedia(const QString &data)
{
    emit mediaChanged(data);

    QStringList arguments;
    arguments << "-slave" << "-quiet" << "-vo" << "directx:noaccel" << data;
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(dataRecieve()));
    m_process->start(MAKE_PLAYER_FULL, arguments);
}

void MusicCoreMPlayer::setPosition(qint64 pos)
{
    if(!m_process)
    {
        return;
    }

    m_process->write(QString("seek %1 2\n").arg(pos).toUtf8());
}

void MusicCoreMPlayer::setLeftVolume()
{
    if(!m_process)
    {
        return;
    }

    m_process->write(QString("af channels=1:1:1\n").toUtf8());
}

void MusicCoreMPlayer::setRightVolume()
{
    if(!m_process)
    {
        return;
    }

    m_process->write(QString("af channels=1:1\n").toUtf8());
}

void MusicCoreMPlayer::setMultiVoice(int number)
{
    if(!m_process)
    {
        return;
    }

    m_process->write(QString("switch_audio %1\n").arg(number).toUtf8());
}

void MusicCoreMPlayer::setMute(bool mute)
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

    emit volumeChanged(value);
    m_process->write(QString("volume %1 1\n").arg(value).toUtf8());
}

bool MusicCoreMPlayer::isPlaying() const
{
    return m_playState == MusicObject::PS_PlayingState;
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
    if(m_playState == MusicObject::PS_StoppedState || m_playState == MusicObject::PS_PausedState)
    {
        m_playState = MusicObject::PS_PlayingState;
        connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(positionRecieve()));
        m_process->write("get_time_pos\n");
    }
    else
    {
        m_playState = MusicObject::PS_PausedState;
        disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(positionRecieve()));
    }

    emit stateChanged(m_playState);
}

void MusicCoreMPlayer::stop()
{
    m_playState = MusicObject::PS_StoppedState;
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
        QByteArray data = m_process->readLine();
        if(data.startsWith("ANS_LENGTH"))
        {
            data.replace(QByteArray("\r\n"), QByteArray(""));
            disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(durationRecieve()));
            emit durationChanged(QString(data).mid(11).toFloat());
            return;
        }
    }
}

void MusicCoreMPlayer::dataRecieve()
{
    switch(m_category)
    {
        case MusicCategory: musicStandardRecieve(); break;
        case VideoCategory: positionRecieve(); break;
        case NullCategory: break;
        default: break;
    }
}

void MusicCoreMPlayer::positionRecieve()
{
    m_timer.start();
    while(m_process->canReadLine())
    {
        QByteArray data = m_process->readLine();
        if(data.startsWith("ANS_TIME_POSITION"))
        {
            data.replace(QByteArray("\r\n"), QByteArray(""));
            emit positionChanged(QString(data).mid(18).toFloat());
        }
    }
}

void MusicCoreMPlayer::musicStandardRecieve()
{
    while(m_process->canReadLine())
    {
        QString message(m_process->readLine());
        if(message.startsWith("ANS_LENGTH"))
        {
            message.replace(QByteArray("\r\n"), QByteArray(""));
            emit durationChanged(QString(message).mid(11).toFloat());
        }
        if(message.startsWith("ANS_TIME_POSITION"))
        {
            message.replace(QByteArray("\r\n"), QByteArray(""));
            emit positionChanged(QString(message).mid(18).toFloat());
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
        emit finished(DEFAULT_LEVEL_NORMAL);
    }
}
