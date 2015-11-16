#include "musiccoremplayer.h"
#include "musicobject.h"

#include <QProcess>

MusicCoreMPlayer::MusicCoreMPlayer(QObject *parent)
    : QObject(parent)
{
    m_process = NULL;
}

MusicCoreMPlayer::~MusicCoreMPlayer()
{
    if(m_process)
    {
        m_process->kill();
    }
    delete m_process;
}

void MusicCoreMPlayer::setMedia(const QString &data, int winId)
{
    if(m_process)
    {
        m_process->kill();
        delete m_process;
        m_process = NULL;
    }

    m_playState = StoppedState;
    m_process = new QProcess(this);
    QStringList arguments;
    arguments << "-slave" << "-quiet" << "-wid";
    arguments << QString::number(winId);
    arguments << "-vo" << "direct3d" << data;
    emit mediaChanged(data);

    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(durationRecieve()));
    m_process->write("get_time_length\n");
    m_process->start(MusicObject::getAppDir() + MAKE_PLAYER, arguments);

}

void MusicCoreMPlayer::setPosition(qint64 pos)
{
    if(!m_process)
    {
        return;
    }
    m_process->write(QString("seek %1").arg(pos).toUtf8());
}

void MusicCoreMPlayer::setMute(bool mute)
{
    if(!m_process)
    {
        return;
    }
    m_process->write(QString("mute %1").arg(mute ? 1 : 0).toUtf8());
}

void MusicCoreMPlayer::setVolume(int value)
{
    if(!m_process)
    {
        return;
    }
    emit volumnChanged(value);
    m_process->write(QString("volume %1 1\n").arg(value).toUtf8());
}

void MusicCoreMPlayer::play()
{
    if(!m_process)
    {
        return;
    }

    m_process->write("pause\n");
    if(m_playState == StoppedState || m_playState == PausedState)
    {
        m_playState = PlayingState;
        connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(positionRecieve()));
        m_process->write("get_time_pos\n");
    }
    else
    {
        m_playState = PausedState;
        disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(positionRecieve()));
    }
    emit stateChanged(m_playState);
}

void MusicCoreMPlayer::durationRecieve()
{
    while(m_process->canReadLine())
    {
        QByteArray data = m_process->readLine();
        if(data.startsWith("ANS_LENGTH"))
        {
            data.replace(QByteArray("\r\n"), "");
            disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(durationRecieve()));
            emit durationChanged(QString(data).mid(11).toFloat());
            return;
        }
    }
}

void MusicCoreMPlayer::positionRecieve()
{
    m_process->write("get_time_length\n");
    m_process->write("get_time_pos\n");
    while(m_process->canReadLine())
    {
        QByteArray data = m_process->readLine();
        if(data.startsWith("ANS_TIME_POSITION"))
        {
            data.replace(QByteArray("\r\n"), "");
            emit positionChanged(QString(data).mid(18).toFloat());
        }
    }
}

void MusicCoreMPlayer::stop()
{
    if(!m_process)
    {
        return;
    }
    m_process->write("quit\n");
}
