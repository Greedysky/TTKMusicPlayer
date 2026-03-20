#include "qmmpevents_p.h"
#include "qmmpaudioengine_p.h"
#include "statehandler.h"
#include "inputsource.h"
#include "volumehandler.h"
#include "metadatamanager.h"
#include "qmmpsettings.h"
#include "soundcore.h"

class SoundCorePrivate
{
public:
    SoundCorePrivate(SoundCore *q)
    {
        if(instance)
            qFatal("SoundCore: only one instance is allowed");

        instance = q;

        handler = new StateHandler(q);
        volumeControl = new VolumeHandler(q);

        QObject::connect(handler, SIGNAL(elapsedChanged(qint64)), q, SIGNAL(elapsedChanged(qint64)));
        QObject::connect(handler, SIGNAL(bitrateChanged(int)), q, SIGNAL(bitrateChanged(int)));
        QObject::connect(handler, SIGNAL(audioParametersChanged(AudioParameters)), q, SIGNAL(audioParametersChanged(AudioParameters)));
        QObject::connect(handler, SIGNAL(bufferingProgress(int)), q, SIGNAL(bufferingProgress(int)));
        QObject::connect(QmmpSettings::instance(), SIGNAL(eqSettingsChanged()), q, SIGNAL(eqSettingsChanged()));
        QObject::connect(QmmpSettings::instance(), SIGNAL(audioSettingsChanged()), volumeControl, SLOT(reload()));
        QObject::connect(volumeControl, SIGNAL(volumeChanged(int)), q, SIGNAL(volumeChanged(int)));
        QObject::connect(volumeControl, SIGNAL(balanceChanged(int)), q, SIGNAL(balanceChanged(int)));
        QObject::connect(volumeControl, SIGNAL(mutedChanged(bool)), q, SIGNAL(mutedChanged(bool)));

    }

    enum NextEngineState
    {
        NO_ENGINE = 0,
        SAME_ENGINE,
        ANOTHER_ENGINE,
        INVALID_SOURCE
    };

    QHash <QString, QString> streamInfo;
    TrackInfo info;
    QString path;
    StateHandler *handler;
    VolumeHandler *volumeControl;
    AbstractEngine *engine = nullptr;
    QQueue<InputSource *> sources;
    NextEngineState nextState = NO_ENGINE;

    static SoundCore *instance;

};

SoundCore *SoundCorePrivate::instance = nullptr;


SoundCore::SoundCore(QObject *parent)
    : QObject(parent),
      d(new SoundCorePrivate(this))
{
    qRegisterMetaType<Qmmp::State>("Qmmp::State");
}

SoundCore::~SoundCore()
{
    stop();
    delete d;
}

bool SoundCore::play(const QString &source, bool queue, qint64 offset)
{
    if(!queue)
        stop();

    MetaDataManager::instance(); //create metadata manager

    InputSource *s = InputSource::create(source, this);
    s->setOffset(offset);
    d->sources.enqueue(s);

    connect(s, SIGNAL(ready()), SLOT(startNextSource()));
    connect(s, SIGNAL(error()), SLOT(startNextSource()));

    if(!s->initialize())
    {
        d->sources.removeAll(s);
        s->deleteLater();
        if(d->handler->state() == Qmmp::Stopped || d->handler->state() == Qmmp::Buffering)
            d->handler->dispatch(Qmmp::NormalError);
        return false;
    }
    if(d->handler->state() == Qmmp::Stopped)
        d->handler->dispatch(Qmmp::Buffering);
    return true;
}

void SoundCore::stop()
{
    qApp->sendPostedEvents(this, 0);
    d->path.clear();
    qDeleteAll(d->sources);
    d->sources.clear();
    d->nextState = SoundCorePrivate::NO_ENGINE;
    if(d->engine)
    {
        d->engine->stop();
        qApp->sendPostedEvents(this, 0);
        //d->engine->deleteLater();
        //d->engine = 0;
    }
    d->volumeControl->reload();
    if(state() == Qmmp::NormalError || state() == Qmmp::FatalError || state() == Qmmp::Buffering)
        StateHandler::instance()->dispatch(Qmmp::Stopped); //clear error and buffering state
}

void SoundCore::pause()
{
    if(d->engine)
        d->engine->pause();
}

void SoundCore::seek(qint64 time)
{
    if(d->engine)
        d->engine->seek(time);
}

void SoundCore::seekRelative(qint64 offset)
{
    const qint64 d = duration();
    if(d > 0)
        seek(qBound(qint64(0), elapsed() + offset, d));
}

bool SoundCore::nextTrackAccepted() const
{
    return d->nextState == SoundCorePrivate::SAME_ENGINE;
}

QString SoundCore::path() const
{
    return d->path;
}

qint64 SoundCore::duration() const
{
    return d->handler->duration();
}

EqSettings SoundCore::eqSettings() const
{
    return QmmpSettings::instance()->eqSettings();
}

void SoundCore::setEqSettings(const EqSettings &settings)
{
    QmmpSettings::instance()->setEqSettings(settings);
}

void SoundCore::setVolumePerChannel(int left, int right)
{
    setMuted(false);
    d->volumeControl->setVolume(left, right);
}

void SoundCore::setMuted(bool mute)
{
    d->volumeControl->setMuted(mute);
}

void SoundCore::changeVolume(int delta)
{
    setMuted(false);
    d->volumeControl->changeVolume(delta);
}

void SoundCore::setVolume(int volume)
{
    setMuted(false);
    d->volumeControl->setVolume(volume);
}

void SoundCore::volumeUp()
{
    changeVolume(QmmpSettings::instance()->volumeStep());
}

void SoundCore::volumeDown()
{
    changeVolume(-QmmpSettings::instance()->volumeStep());
}

void SoundCore::setBalance(int balance)
{
    setMuted(false);
    d->volumeControl->setBalance(balance);
}

int SoundCore::leftVolume() const
{
    return d->volumeControl->left();
}

int SoundCore::rightVolume() const
{
    return d->volumeControl->right();
}

int SoundCore::volume() const
{
    return d->volumeControl->volume();
}

int SoundCore::balance() const
{
    return d->volumeControl->balance();
}

bool SoundCore::isMuted() const
{
    return d->volumeControl->isMuted();
}

qint64 SoundCore::elapsed() const
{
    return d->handler->elapsed();
}

int SoundCore::bitrate() const
{
    return d->handler->bitrate();
}

AudioParameters SoundCore::audioParameters() const
{
    return d->handler->audioParameters();
}

Qmmp::State SoundCore::state() const
{
    return d->handler->state();
}

const QMap<Qmmp::MetaData, QString> &SoundCore::metaData() const
{
    return d->info.metaData();
}

QString SoundCore::metaData(Qmmp::MetaData key) const
{
    return d->info.value(key);
}

const QHash<QString, QString> &SoundCore::streamInfo() const
{
    return d->streamInfo;
}

const TrackInfo &SoundCore::trackInfo() const
{
    return d->info;
}

SoundCore* SoundCore::instance()
{
    return SoundCorePrivate::instance;
}

void SoundCore::startNextSource()
{
    if(d->sources.isEmpty())
        return;

    InputSource *s = d->sources.dequeue();
    d->path = s->path();

    if(s->ioDevice() && !s->ioDevice()->isOpen() && !s->ioDevice()->open(QIODevice::ReadOnly))
    {
        qWarning("SoundCore: input error: %s", qPrintable(s->ioDevice()->errorString()));
        d->path.clear();
        s->deleteLater();
        d->nextState = SoundCorePrivate::INVALID_SOURCE;
        if(state() == Qmmp::Stopped || state() == Qmmp::Buffering)
            d->handler->dispatch(Qmmp::NormalError);
        return;
    }

    if(!d->engine)
    {
        if((d->engine = AbstractEngine::create(s, this)))
        {
            d->engine->play();
            d->nextState = SoundCorePrivate::NO_ENGINE;
            return;
        }

        s->deleteLater();
        d->handler->dispatch(Qmmp::NormalError);
        return;
    }

    if(AbstractEngine::isEnabled(d->engine) && d->engine->enqueue(s))
    {
        if(state() == Qmmp::Stopped || state() == Qmmp::Buffering)
        {
            d->engine->play();
            d->nextState = SoundCorePrivate::NO_ENGINE;
        }
        else
        {
            d->nextState = SoundCorePrivate::SAME_ENGINE;
        }
    }
    else
    {
        d->sources.prepend(s); //try next engine
        d->nextState = SoundCorePrivate::ANOTHER_ENGINE;
        if(state() == Qmmp::Stopped || state() == Qmmp::Buffering)
        {
            startNextEngine();
        }
    }
}

void SoundCore::startNextEngine()
{
    switch(d->nextState)
    {
    case SoundCorePrivate::NO_ENGINE:
    case SoundCorePrivate::SAME_ENGINE:
    {
        if(d->sources.isEmpty())
            d->nextState = SoundCorePrivate::NO_ENGINE;
        else if(!d->sources.first()->isReady() && state() == Qmmp::Stopped)
            d->handler->dispatch(Qmmp::Buffering);
        break;
    }
    case SoundCorePrivate::ANOTHER_ENGINE:
    {
        d->nextState = SoundCorePrivate::NO_ENGINE;
        if(d->engine)
        {
            d->engine->deleteLater();
            d->engine = nullptr;
        }
        if(!d->sources.isEmpty())
        {
            d->handler->dispatch(Qmmp::Buffering);
            startNextSource();
        }
        break;
    }
    case SoundCorePrivate::INVALID_SOURCE:
        d->handler->dispatch(Qmmp::NormalError);
    }
}

bool SoundCore::event(QEvent *e)
{
    if(e->type() == EVENT_STATE_CHANGED)
    {
        Qmmp::State st = ((StateChangedEvent *) e)->currentState();
        emit stateChanged(st);
        if(st == Qmmp::Stopped)
        {
            d->streamInfo.clear();
            startNextEngine();
        }
    }
    else if(e->type() == EVENT_STREAM_INFO_CHANGED)
    {
        d->streamInfo = ((StreamInfoChangedEvent *) e)->streamInfo();
        emit streamInfoChanged();
    }
    else if(e->type() == EVENT_TRACK_INFO_CHANGED)
    {
        d->info = ((TrackInfoEvent *) e)->trackInfo();
        emit trackInfoChanged();
    }
    else if(e->type() == EVENT_NEXT_TRACK_REQUEST)
        emit nextTrackRequest();
    else if(e->type() == EVENT_FINISHED)
        emit finished();
    return QObject::event(e);
}
