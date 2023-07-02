#include <QApplication>
#include <QSettings>
#include <QDir>
#include "qmmpevents_p.h"
#include "qmmpaudioengine_p.h"
#include "decoderfactory.h"
#include "effect.h"
#include "statehandler.h"
#include "inputsource.h"
#include "volumehandler.h"
#include "enginefactory.h"
#include "metadatamanager.h"
#include "qmmpsettings.h"
#include "soundcore.h"

SoundCore *SoundCore::m_instance = nullptr;

SoundCore::SoundCore(QObject *parent)
    : QObject(parent)
{
    if(m_instance)
        qFatal("SoundCore: only one instance is allowed");
    m_instance = this;

    qRegisterMetaType<Qmmp::State>("Qmmp::State");
    m_handler = new StateHandler(this);
    m_volumeControl = new VolumeHandler(this);

    connect(m_handler, SIGNAL(elapsedChanged(qint64)), SIGNAL(elapsedChanged(qint64)));
    connect(m_handler, SIGNAL(bitrateChanged(int)), SIGNAL(bitrateChanged(int)));
    connect(m_handler, SIGNAL(audioParametersChanged(AudioParameters)), SIGNAL(audioParametersChanged(AudioParameters)));
    connect(m_handler, SIGNAL(bufferingProgress(int)), SIGNAL(bufferingProgress(int)));
    connect(QmmpSettings::instance(), SIGNAL(eqSettingsChanged()), SIGNAL(eqSettingsChanged()));
    connect(QmmpSettings::instance(), SIGNAL(audioSettingsChanged()), m_volumeControl, SLOT(reload()));
    connect(m_volumeControl, SIGNAL(volumeChanged(int, int)), SIGNAL(volumeChanged(int, int)));
    connect(m_volumeControl, SIGNAL(volumeChanged(int)), SIGNAL(volumeChanged(int)));
    connect(m_volumeControl, SIGNAL(balanceChanged(int)), SIGNAL(balanceChanged(int)));
    connect(m_volumeControl, SIGNAL(mutedChanged(bool)), SIGNAL(mutedChanged(bool)));
}

SoundCore::~SoundCore()
{
    stop();
    m_instance = nullptr;
}

bool SoundCore::play(const QString &source, bool queue, qint64 offset)
{
    if(!queue)
        stop();

    MetaDataManager::instance(); //create metadata manager

    InputSource *s = InputSource::create(source, this);
    s->setOffset(offset);
    m_sources.enqueue(s);

    connect(s, SIGNAL(ready()), SLOT(startNextSource()));
    connect(s, SIGNAL(error()), SLOT(startNextSource()));

    if(!s->initialize())
    {
        m_sources.removeAll(s);
        s->deleteLater();
        if(m_handler->state() == Qmmp::Stopped || m_handler->state() == Qmmp::Buffering)
            m_handler->dispatch(Qmmp::NormalError);
        return false;
    }
    if(m_handler->state() == Qmmp::Stopped)
        m_handler->dispatch(Qmmp::Buffering);
    return true;
}

void SoundCore::stop()
{
    qApp->sendPostedEvents(this, 0);
    m_path.clear();
    qDeleteAll(m_sources);
    m_sources.clear();
    m_nextState = NO_ENGINE;
    if(m_engine)
    {
        m_engine->stop();
        qApp->sendPostedEvents(this, 0);
        //m_engine->deleteLater();
        //m_engine = 0;
    }
    m_volumeControl->reload();
    if(state() == Qmmp::NormalError || state() == Qmmp::FatalError || state() == Qmmp::Buffering)
        StateHandler::instance()->dispatch(Qmmp::Stopped); //clear error and buffering state
}

void SoundCore::pause()
{
    if(m_engine)
        m_engine->pause();
}

void SoundCore::seek(qint64 time)
{
    if(m_engine)
        m_engine->seek(time);
}

const QString SoundCore::path() const
{
    return m_path;
}

bool SoundCore::nextTrackAccepted() const
{
    return m_nextState == SAME_ENGINE;
}

qint64 SoundCore::duration() const
{
    return m_handler->duration();
}

EqSettings SoundCore::eqSettings() const
{
    return QmmpSettings::instance()->eqSettings();
}

void SoundCore::setEqSettings(const EqSettings &settings)
{
    QmmpSettings::instance()->setEqSettings(settings);
}

void SoundCore::setVolume(int L, int R)
{
    setMuted(false);
    m_volumeControl->setVolume(L, R);
}

void SoundCore::setMuted(bool mute)
{
    m_volumeControl->setMuted(mute);
}

void SoundCore::changeVolume(int delta)
{
    setMuted(false);
    m_volumeControl->changeVolume(delta);
}

void SoundCore::setVolume(int volume)
{
    setMuted(false);
    m_volumeControl->setVolume(volume);
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
    m_volumeControl->setBalance(balance);
}

int SoundCore::leftVolume() const
{
    return m_volumeControl->left();
}

int SoundCore::rightVolume() const
{
    return m_volumeControl->right();
}

int SoundCore::volume() const
{
    return m_volumeControl->volume();
}

int SoundCore::balance() const
{
    return m_volumeControl->balance();
}

bool SoundCore::isMuted() const
{
    return m_volumeControl->isMuted();
}

qint64 SoundCore::elapsed() const
{
    return m_handler->elapsed();
}

int SoundCore::bitrate() const
{
    return m_handler->bitrate();
}

AudioParameters SoundCore::audioParameters() const
{
    return m_handler->audioParameters();
}

Qmmp::State SoundCore::state() const
{
    return m_handler->state();
}

const QMap<Qmmp::MetaData, QString> &SoundCore::metaData() const
{
    return m_info.metaData();
}

QString SoundCore::metaData(Qmmp::MetaData key) const
{
    return m_info.value(key);
}

const QHash<QString, QString> &SoundCore::streamInfo() const
{
    return m_streamInfo;
}

const TrackInfo &SoundCore::trackInfo() const
{
    return m_info;
}

void SoundCore::startNextSource()
{
    if(m_sources.isEmpty())
        return;

    InputSource *s = m_sources.dequeue();
    m_path = s->path();

    if(s->ioDevice() && !s->ioDevice()->isOpen() && !s->ioDevice()->open(QIODevice::ReadOnly))
    {
        qWarning("SoundCore: input error: %s", qPrintable(s->ioDevice()->errorString()));
        m_path.clear();
        s->deleteLater();
        m_nextState = INVALID_SOURCE;
        if(m_handler->state() == Qmmp::Stopped || m_handler->state() == Qmmp::Buffering)
            m_handler->dispatch(Qmmp::NormalError);
        return;
    }

    if(!m_engine)
    {
        if((m_engine = AbstractEngine::create(s, this)))
        {
            m_engine->play();
            m_nextState = NO_ENGINE;
            return;
        }
        else
        {
            s->deleteLater();
            m_handler->dispatch(Qmmp::NormalError);
            return;
        }
    }

    if(AbstractEngine::isEnabled(m_engine) && m_engine->enqueue(s))
    {
        if(state() == Qmmp::Stopped || state() == Qmmp::Buffering)
        {
            m_engine->play();
            m_nextState = NO_ENGINE;
        }
        else
        {
            m_nextState = SAME_ENGINE;
        }
    }
    else
    {
        m_sources.prepend(s); //try next engine
        m_nextState = ANOTHER_ENGINE;
        if(state() == Qmmp::Stopped || state() == Qmmp::Buffering)
        {
            startNextEngine();
        }
    }
}

void SoundCore::startNextEngine()
{
    switch(m_nextState)
    {
    case NO_ENGINE:
    case SAME_ENGINE:
    {
        if(m_sources.isEmpty())
            m_nextState = NO_ENGINE;
        else if(!m_sources.front()->isReady() && state() == Qmmp::Stopped)
            m_handler->dispatch(Qmmp::Buffering);
        break;
    }
    case ANOTHER_ENGINE:
    {
        m_nextState = NO_ENGINE;
        if(m_engine)
        {
            m_engine->deleteLater();
            m_engine = nullptr;
        }
        if(!m_sources.isEmpty())
        {
            m_handler->dispatch(Qmmp::Buffering);
            startNextSource();
        }
        break;
    }
    case INVALID_SOURCE:
        m_handler->dispatch(Qmmp::NormalError);
    }
}

SoundCore* SoundCore::instance()
{
    return m_instance;
}

bool SoundCore::event(QEvent *e)
{
    if(e->type() == EVENT_STATE_CHANGED)
    {
        Qmmp::State st = ((StateChangedEvent *) e)->currentState();
        emit stateChanged(st);
        if(st == Qmmp::Stopped)
        {
            m_streamInfo.clear();
            startNextEngine();
        }
    }
    else if(e->type() == EVENT_STREAM_INFO_CHANGED)
    {
        m_streamInfo = ((StreamInfoChangedEvent *) e)->streamInfo();
        emit streamInfoChanged();
    }
    else if(e->type() == EVENT_TRACK_INFO_CHANGED)
    {
        m_info = ((TrackInfoEvent *) e)->trackInfo();
        emit trackInfoChanged();
    }
    else if(e->type() == EVENT_NEXT_TRACK_REQUEST)
        emit nextTrackRequest();
    else if(e->type() == EVENT_FINISHED)
        emit finished();
    return QObject::event(e);
}
