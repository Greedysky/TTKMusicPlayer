#include <QApplication>
#include "soundcore.h"
#include "qmmpevents_p.h"
#include "statehandler.h"
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
#  include <QMutex>
#else
#  include <QRecursiveMutex>
#endif

#define TICK_INTERVAL 250
#define PREFINISH_TIME 7000

class StateHandlerPrivate
{
public:
    StateHandlerPrivate(StateHandler *q)
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        : mutex(QMutex::Recursive)
#endif
    {
        if(instance)
            qFatal("StateHandler: only one instance is allowed");

        instance = q;
    }

    ~StateHandlerPrivate()
    {
        instance = nullptr;
    }

    qint64 elapsed = -1;
    qint64 duration = 0;
    bool sendAboutToFinish = true;
    int bitrate = 0;
    TrackInfo info;
    QHash <QString, QString> streamInfo;
    Qmmp::State state = Qmmp::Stopped;
    AudioParameters audioParameters;
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
    mutable QMutex mutex;
#else
    mutable QRecursiveMutex mutex;
#endif

    static StateHandler *instance;

};

StateHandler* StateHandlerPrivate::instance = nullptr;


StateHandler::StateHandler(QObject *parent)
    : QObject(parent),
      d(new StateHandlerPrivate(this))
{
    qRegisterMetaType<AudioParameters>("AudioParameters");
}

StateHandler::~StateHandler()
{
    delete d;
}

void StateHandler::dispatch(qint64 elapsed, int bitrate)
{
    d->mutex.lock();
    if(std::abs(d->elapsed - elapsed) > TICK_INTERVAL)
    {
        d->elapsed = elapsed;
        emit elapsedChanged(elapsed);
        if(d->bitrate != bitrate)
        {
            d->bitrate = bitrate;
            emit bitrateChanged(bitrate);
        }
        if((SoundCore::instance()->duration() > PREFINISH_TIME)
                 && (d->duration - d->elapsed < PREFINISH_TIME)
                 && d->sendAboutToFinish)
        {
            d->sendAboutToFinish = false;
            if(d->duration - d->elapsed > PREFINISH_TIME / 2)
                qApp->postEvent(parent(), new QEvent(EVENT_NEXT_TRACK_REQUEST));
        }
    }
    d->mutex.unlock();
}

void StateHandler::dispatch(const AudioParameters &p)
{
    d->mutex.lock();
    if(d->audioParameters != p)
    {
        d->audioParameters = p;
        emit audioParametersChanged(p);
    }
    d->mutex.unlock();
}

void StateHandler::dispatch(qint64 length)
{
    d->mutex.lock();
    d->duration = length;
    d->mutex.unlock();
}

bool StateHandler::dispatch(const TrackInfo &info)
{
    QMutexLocker locker(&d->mutex);
    if(info.isEmpty())
    {
        qWarning("StateHandler: empty metadata");
        return false;
    }
    if(d->state != Qmmp::Playing && d->state != Qmmp::Paused)
    {
        qWarning("StateHandler: metadata is ignored");
        return false;
    }

    if(d->info.isEmpty() || d->info.path() == info.path())
    {
        TrackInfo tmp = d->info;
        tmp.setPath(info.path());
        if(info.parts() & TrackInfo::MetaData)
            tmp.setValues(info.metaData());
        if(info.parts() & TrackInfo::Properties)
            tmp.setValues(info.properties());
        if(info.parts() & TrackInfo::ReplayGainInfo)
            tmp.setValues(info.replayGainInfo());
        if(info.duration() > 0)
            tmp.setDuration(info.duration());

        if(d->info != tmp)
        {
            d->info = tmp;
            qApp->postEvent(parent(), new TrackInfoEvent(d->info));
            return true;
        }
    }
    return false;
}

void StateHandler::dispatch(const QHash<QString, QString> &info)
{
    d->mutex.lock();
    QHash<QString, QString> tmp = info;
    const auto values = tmp.values();
    for(const QString &value : qAsConst(values)) //remove empty keys
    {
        if(value.isEmpty())
            tmp.remove(tmp.key(value));
    }
    if(d->streamInfo != tmp)
    {
        d->streamInfo = tmp;
        qApp->postEvent(parent(), new StreamInfoChangedEvent(d->streamInfo));
    }
    d->mutex.unlock();
}

void StateHandler::dispatch(Qmmp::State state)
{
    d->mutex.lock();
    //clear
    static const QList<Qmmp::State> clearStates = { Qmmp::Stopped, Qmmp::NormalError, Qmmp::FatalError };
    if(clearStates.contains(state))
    {
        d->elapsed = -1;
        d->bitrate = 0;
        d->info.clear();
        d->streamInfo.clear();
        d->sendAboutToFinish = true;
        d->audioParameters = AudioParameters(44100, ChannelMap(2), Qmmp::PCM_UNKNOWN);
    }

    if(d->state != state)
    {
        static const QStringList states = { "Playing", "Paused", "Stopped", "Buffering", "NormalError", "FatalError" };
        qDebug("StateHandler: Current state: %s; previous state: %s",
               qPrintable(states.at(state)), qPrintable(states.at(d->state)));

        if(d->state == Qmmp::Stopped && state == Qmmp::Paused)
        {
            qDebug("StateHandler: Error switch state from %s to %s",
                   qPrintable(states.at(d->state)), qPrintable(states.at(state)));
        }
        else
        {
            Qmmp::State prevState = d->state;
            d->state = state;
            qApp->postEvent(parent(), new StateChangedEvent(d->state, prevState));
        }
    }
    d->mutex.unlock();
}

void StateHandler::dispatchBuffer(int percent)
{
    if(d->state == Qmmp::Buffering)
        emit bufferingProgress(percent);
}

qint64 StateHandler::elapsed() const
{
    QMutexLocker locker(&d->mutex);
    return d->elapsed;
}

qint64 StateHandler::duration() const
{
    QMutexLocker locker(&d->mutex);
    return d->duration;
}

int StateHandler::bitrate() const
{
    QMutexLocker locker(&d->mutex);
    return d->bitrate;
}

AudioParameters StateHandler::audioParameters() const
{
    QMutexLocker locker(&d->mutex);
    return d->audioParameters;
}

Qmmp::State StateHandler::state() const
{
    return d->state;
}

void StateHandler::sendNextTrackRequest()
{
    d->mutex.lock();
    if(d->sendAboutToFinish)
    {
        d->sendAboutToFinish = false;
        qApp->postEvent(parent(), new QEvent(EVENT_NEXT_TRACK_REQUEST));
    }
    d->mutex.unlock();
}

void StateHandler::sendFinished()
{
    qApp->postEvent(parent(), new QEvent(EVENT_FINISHED));
}

StateHandler *StateHandler::instance()
{
    return StateHandlerPrivate::instance;
}
