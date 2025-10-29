#include <QStringList>
#include <QApplication>
#include <QMutexLocker>
#include <QMetaType>
#include "soundcore.h"
#include "qmmpevents_p.h"
#include "statehandler.h"

#define TICK_INTERVAL 250
#define PREFINISH_TIME 7000

StateHandler* StateHandler::m_instance = nullptr;

StateHandler::StateHandler(QObject *parent)
    : QObject(parent)
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
    , m_mutex(QMutex::Recursive)
#endif
{
    if(m_instance)
        qFatal("StateHandler: only one instance is allowed");
    qRegisterMetaType<AudioParameters>("AudioParameters");
    m_instance = this;
}

StateHandler::~StateHandler()
{
    m_instance = nullptr;
}

void StateHandler::dispatch(qint64 elapsed, int bitrate)
{
    m_mutex.lock();
    if(std::abs(m_elapsed - elapsed) > TICK_INTERVAL)
    {
        m_elapsed = elapsed;
        emit elapsedChanged(elapsed);
        if(m_bitrate != bitrate)
        {
            m_bitrate = bitrate;
            emit bitrateChanged(bitrate);
        }
        if((SoundCore::instance()->duration() > PREFINISH_TIME)
                 && (m_duration - m_elapsed < PREFINISH_TIME)
                 && m_sendAboutToFinish)
        {
            m_sendAboutToFinish = false;
            if(m_duration - m_elapsed > PREFINISH_TIME / 2)
                qApp->postEvent(parent(), new QEvent(EVENT_NEXT_TRACK_REQUEST));
        }
    }
    m_mutex.unlock();
}

void StateHandler::dispatch(const AudioParameters &p)
{
    m_mutex.lock();
    if(m_audioParameters != p)
    {
        m_audioParameters = p;
        emit audioParametersChanged(p);
    }
    m_mutex.unlock();
}

void StateHandler::dispatch(qint64 length)
{
    m_mutex.lock();
    m_duration = length;
    m_mutex.unlock();
}

bool StateHandler::dispatch(const TrackInfo &info)
{
    QMutexLocker locker(&m_mutex);
    if(info.isEmpty())
    {
        qWarning("StateHandler: empty metadata");
        return false;
    }
    if(m_state != Qmmp::Playing && m_state != Qmmp::Paused)
    {
        qWarning("StateHandler: metadata is ignored");
        return false;
    }

    if(m_info.isEmpty() || m_info.path() == info.path())
    {
        TrackInfo tmp = m_info;
        tmp.setPath(info.path());
        if(info.parts() & TrackInfo::MetaData)
            tmp.setValues(info.metaData());
        if(info.parts() & TrackInfo::Properties)
            tmp.setValues(info.properties());
        if(info.parts() & TrackInfo::ReplayGainInfo)
            tmp.setValues(info.replayGainInfo());
        if(info.duration() > 0)
            tmp.setDuration(info.duration());

        if(m_info != tmp)
        {
            m_info = tmp;
            qApp->postEvent(parent(), new TrackInfoEvent(m_info));
            return true;
        }
    }
    return false;
}

void StateHandler::dispatch(const QHash<QString, QString> &info)
{
    m_mutex.lock();
    QHash<QString, QString> tmp = info;
    const auto values = tmp.values();
    for(const QString &value : qAsConst(values)) //remove empty keys
    {
        if(value.isEmpty())
            tmp.remove(tmp.key(value));
    }
    if(m_streamInfo != tmp)
    {
        m_streamInfo = tmp;
        qApp->postEvent(parent(), new StreamInfoChangedEvent(m_streamInfo));
    }
    m_mutex.unlock();
}

void StateHandler::dispatch(Qmmp::State state)
{
    m_mutex.lock();
    //clear
    static const QList<Qmmp::State> clearStates = { Qmmp::Stopped, Qmmp::NormalError, Qmmp::FatalError };
    if(clearStates.contains(state))
    {
        m_elapsed = -1;
        m_bitrate = 0;
        m_info.clear();
        m_streamInfo.clear();
        m_sendAboutToFinish = true;
        m_audioParameters = AudioParameters(44100, ChannelMap(2), Qmmp::PCM_UNKNOWN);
    }
    if(m_state != state)
    {
        static const QStringList states = { "Playing", "Paused", "Stopped", "Buffering", "NormalError", "FatalError" };
        qDebug("StateHandler: Current state: %s; previous state: %s",
               qPrintable(states.at(state)), qPrintable(states.at(m_state)));
        Qmmp::State prevState = state;
        m_state = state;
        qApp->postEvent(parent(), new StateChangedEvent(m_state, prevState));
    }
    m_mutex.unlock();
}

void StateHandler::dispatchBuffer(int percent)
{
    if(m_state == Qmmp::Buffering)
        emit bufferingProgress(percent);
}

qint64 StateHandler::elapsed() const
{
    QMutexLocker locker(&m_mutex);
    return m_elapsed;
}

qint64 StateHandler::duration() const
{
    QMutexLocker locker(&m_mutex);
    return m_duration;
}

int StateHandler::bitrate() const
{
    QMutexLocker locker(&m_mutex);
    return m_bitrate;
}

AudioParameters StateHandler::audioParameters() const
{
    QMutexLocker locker(&m_mutex);
    return m_audioParameters;
}

Qmmp::State StateHandler::state() const
{
    return m_state;
}

void StateHandler::sendNextTrackRequest()
{
    m_mutex.lock();
    if(m_sendAboutToFinish)
    {
        m_sendAboutToFinish = false;
        qApp->postEvent(parent(), new QEvent(EVENT_NEXT_TRACK_REQUEST));
    }
    m_mutex.unlock();
}

void StateHandler::sendFinished()
{
    qApp->postEvent(parent(), new QEvent(EVENT_FINISHED));
}

StateHandler *StateHandler::instance()
{
    return m_instance;
}
