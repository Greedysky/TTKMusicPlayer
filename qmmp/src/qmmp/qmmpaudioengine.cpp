#include <QMetaType>
#include <QIODevice>
#include "replaygain_p.h"
#include "effect.h"
#include "buffer.h"
#include "decoder.h"
#include "outputwriter_p.h"
#include "decoderfactory.h"
#include "effectfactory.h"
#include "inputsource.h"
#include "statehandler.h"
#include "audioconverter.h"
#include "qmmpaudioengine_p.h"
#include "channelconverter_p.h"
#include "dithering_p.h"
#include "metadatamanager.h"

#define TRANSPORT_TIMEOUT 5000 //ms

QmmpAudioEngine::QmmpAudioEngine(QObject *parent)
    : AbstractEngine(parent)
{
    m_converter = new AudioConverter;
    m_settings = QmmpSettings::instance();

    connect(m_settings,SIGNAL(replayGainSettingsChanged()), SLOT(updateReplayGainSettings()));
    connect(m_settings,SIGNAL(audioSettingsChanged()), SLOT(updateAudioSettings()));
    connect(m_settings, SIGNAL(eqSettingsChanged()), SLOT(updateEqSettings()));
    reset();
    m_instance = this;
}

QmmpAudioEngine::~QmmpAudioEngine()
{
    QmmpAudioEngine::stop();
    reset();
    if(m_output_buf)
        delete[] m_output_buf;
    m_output_buf = nullptr;
    qDeleteAll(m_effects);
    m_instance = nullptr;
    delete m_converter;
}

void QmmpAudioEngine::reset()
{
    m_done = false;
    m_finish = false;
    m_seekTime = -1;
    m_output_at = 0;
    m_user_stop = false;
    m_bitrate = 0;
    m_next = false;
}

void QmmpAudioEngine::clearDecoders()
{
    if(m_decoder)
    {
        m_inputs.take(m_decoder)->deleteLater();
        delete m_decoder;
        m_decoder = nullptr;
    }
    while(!m_decoders.isEmpty())
    {
        Decoder *d = m_decoders.dequeue();
        m_inputs.take(d)->deleteLater();
        delete d;
    }
}

bool QmmpAudioEngine::play()
{
    if(isRunning() || m_decoders.isEmpty() || (m_output && m_output->isRunning()))
        return false;
    if(m_output)
        delete m_output;
    prepareEffects(m_decoders.head());
    if(!(m_output = createOutput()))
        return false;
    m_dithering->setFormats(m_decoders.head()->audioParameters().format(), m_output->outputAudioParameters().format());
    reset();
#ifdef Q_OS_WIN
    start(QThread::HighPriority);
#else
    start();
#endif
    return true;
}

bool QmmpAudioEngine::enqueue(InputSource *source)
{
    mutex()->lock();
    if(m_decoder && m_decoder->nextURL() == source->path())
    {
        m_inputs.value(m_decoder)->setOffset(source->offset());
        delete source;
        m_next = true;
        mutex()->unlock();
        return true;
    }
    mutex()->unlock();

    DecoderFactory *factory = nullptr;

    if(!source->path().contains("://"))
        factory = Decoder::findByFilePath(source->path(), m_settings->determineFileTypeByContent());
    if(!factory)
        factory = Decoder::findByMime(source->contentType());
    if(factory && !factory->properties().noInput && source->ioDevice() && source->path().contains("://"))
        factory = (factory->canDecode(source->ioDevice()) ? factory : nullptr);
    if(!factory && source->ioDevice() && source->path().contains("://")) //ignore content of local files
        factory = Decoder::findByContent(source->ioDevice());
    if(!factory && source->path().contains("://"))
        factory = Decoder::findByProtocol(source->path().section("://", 0, 0));
    if(!factory)
    {
        qWarning("QmmpAudioEngine: unsupported file format");
        return false;
    }
    qDebug("QmmpAudioEngine: selected decoder: %s",qPrintable(factory->properties().shortName));
    if(factory->properties().noInput && source->ioDevice())
        source->ioDevice()->close();
    Decoder *decoder = factory->create(source->path(), source->ioDevice());
    if(!decoder->initialize())
    {
        qWarning("QmmpAudioEngine: invalid file format");
        delete decoder;
        return false;
    }
    attachMetaData(decoder, factory, source);
    mutex()->lock();
    m_decoders.enqueue(decoder);
    m_inputs.insert(decoder, source);
    mutex()->unlock();
    if(!decoder->totalTime())
        source->setOffset(-1);
    source->setParent(this);
    return true;
}

void QmmpAudioEngine::addEffect(EffectFactory *factory)
{
    for(const Effect *effect : qAsConst(m_effects))
    {
        if(effect->factory() == factory)
        {
            qWarning("QmmpAudioEngine: effect %s already exists", qPrintable(factory->properties().shortName));
            return;
        }
    }

    if(m_output && m_output->isRunning())
    {
        Effect *effect = Effect::create(factory);
        if(!effect)
            return;
        effect->configure(m_ap.sampleRate(), m_ap.channelMap());
        if(effect->audioParameters() == m_ap)
        {
            mutex()->lock();
            m_effects << effect;
            mutex()->unlock();
        }
        else
        {
            qDebug("QmmpAudioEngine: restart is required %s", qPrintable(factory->properties().shortName));
            delete effect;
        }
    }
}

void QmmpAudioEngine::removeEffect(EffectFactory *factory)
{
    Effect *effect = nullptr;
    for(Effect *e : qAsConst(m_effects))
    {
        if(e->factory() == factory)
        {
            effect = e;
            break;
        }
    }
    if(!effect)
        return;
    if(m_output && m_output->isRunning())
    {
        mutex()->lock();
        if(m_blockedEffects.contains(effect))
            qDebug("QmmpAudioEngine: restart is required");
        else
            m_effects.removeAll(effect);
        mutex()->unlock();
    }
}

void QmmpAudioEngine::seek(qint64 time)
{
    if(m_output && m_output->isRunning())
    {
        m_output->seek(time, true);
        if(isRunning())
        {
            mutex()->lock();
            m_seekTime = time;
            mutex()->unlock();
        }
    }
}

void QmmpAudioEngine::pause()
{
    if(m_output)
        m_output->pause();

    if(m_output)
    {
        m_output->recycler()->mutex()->lock();
        m_output->recycler()->cond()->wakeAll();
        m_output->recycler()->mutex()->unlock();
    }
}

void QmmpAudioEngine::stop()
{
    m_user_stop = true;

    if(m_output)
        m_output->recycler()->cond()->wakeAll();

    if(isRunning())
        wait();

    if(m_output)
    {
        delete m_output;
        m_output = nullptr;
    }

    clearDecoders();
    reset();
    while(!m_effects.isEmpty()) //delete effects
        delete m_effects.takeFirst();
    m_replayGain = nullptr;
    m_dithering = nullptr;
}

qint64 QmmpAudioEngine::produceSound(unsigned char *data, qint64 size, quint32 brate)
{
    Buffer *b = m_output->recycler()->get();
    b->trackInfo = m_trackInfo;
    m_trackInfo.clear();
    size_t sz = size < m_bks ? size : m_bks;
    size_t samples = sz / m_sample_size;

    m_converter->toFloat(data, b->data, samples);

    b->samples = samples;
    b->rate = brate;
    for(Effect *effect : qAsConst(m_effects))
    {
        effect->applyEffect(b);
    }
    memmove(data, data + sz, size - sz);
    m_output->recycler()->add();
    return sz;
}

void QmmpAudioEngine::finish()
{
    if(m_output)
    {
        m_output->finish();
    }
    StateHandler::instance()->sendFinished();
}

void QmmpAudioEngine::updateReplayGainSettings()
{
    if(m_replayGain)
    {
        mutex()->lock();
        m_replayGain->updateSettings(m_settings->replayGainMode(),
                                     m_settings->replayGainPreamp(),
                                     m_settings->replayGainDefaultGain(),
                                     m_settings->replayGainPreventClipping());
        mutex()->unlock();
    }
}

void QmmpAudioEngine::updateAudioSettings()
{
    if(m_dithering)
    {
        mutex()->lock();
        m_dithering->setEnabled(m_settings->useDithering());
        mutex()->unlock();
    }
}

void QmmpAudioEngine::updateEqSettings()
{
    mutex()->lock();
    if(m_output)
        m_output->updateEqSettings();
    mutex()->unlock();
}

void QmmpAudioEngine::run()
{
    mutex()->lock();
    m_next = false;
    m_trackInfo.clear();
    qint64 len = 0;
    int delay = 0;
    QString nextURL;
    if(m_decoders.isEmpty())
    {
         mutex()->unlock();
         return;
    }
    m_decoder = m_decoders.dequeue();
    addOffset(); //offset
    mutex()->unlock();
    m_output->start();
    StateHandler::instance()->dispatch(Qmmp::Buffering);
    StateHandler::instance()->dispatch(m_decoder->totalTime());
    StateHandler::instance()->dispatch(Qmmp::Playing);

    while(!m_done && !m_finish)
    {
        mutex()->lock();
        //seek
        if(m_seekTime >= 0)
        {
            m_output->recycler()->mutex()->lock();
            m_output->recycler()->clear();
            m_output->recycler()->mutex()->unlock();
            m_decoder->seek(m_seekTime);
            m_seekTime = -1;
            m_output_at = 0;
        }
        //metadata
        if(m_inputs[m_decoder]->hasMetaData())
            m_decoder->addMetaData(m_inputs[m_decoder]->takeMetaData());

        if(m_inputs[m_decoder]->hasStreamInfo())
            StateHandler::instance()->dispatch(m_inputs[m_decoder]->takeStreamInfo());

        if(m_decoder->hasMetaData())
        {
            QMap<Qmmp::MetaData, QString> m = m_decoder->takeMetaData();
            TrackInfo info(nextURL.isEmpty() ? m_inputs[m_decoder]->path() : nextURL);
            nextURL.clear();
            info.setValues(m);
            info.setValues(m_decoder->properties());
            info.updateValues(m_inputs[m_decoder]->properties());
            info.setValues(m_decoder->replayGainInfo());
            info.setDuration(m_decoder->totalTime());
            if(StateHandler::instance()->dispatch(info))
                m_trackInfo = QSharedPointer<TrackInfo>(new TrackInfo(info));
        }
        //wait more data
        if(m_inputs[m_decoder]->isWaiting())
        {
            mutex()->unlock();
            msleep(5);
            delay += 5;
            m_done = m_user_stop.load();
            if(delay > TRANSPORT_TIMEOUT)
            {
                qWarning("QmmpAudioEngine: unable to receive more data");
                m_done = true;
                StateHandler::instance()->dispatch(Qmmp::NormalError);
                break;
            }
            else
                continue;
        }
        else
        {
            delay = 0;
            // decode
            mutex()->unlock();
            len = m_decoder->read((m_output_buf + m_output_at), m_output_size - m_output_at);
            mutex()->lock();
        }

        if(len > 0)
        {
            m_bitrate = m_decoder->bitrate();
            m_output_at += len;
            flush();
        }
        else if(len == 0)
        {
            if(m_next) //decoder can play next track without initialization
            {
                m_next = false;
                qDebug("QmmpAudioEngine: switching to the next track");
                StateHandler::instance()->sendFinished();
                StateHandler::instance()->dispatch(Qmmp::Stopped); //fake stop/start cycle
                StateHandler::instance()->dispatch(Qmmp::Buffering);
                nextURL = m_decoder->nextURL();
                m_decoder->next();
                StateHandler::instance()->dispatch(m_decoder->totalTime());
                m_replayGain->setReplayGainInfo(m_decoder->replayGainInfo());
                m_output->seek(0); //reset counter
                addOffset(); //offset
                StateHandler::instance()->dispatch(Qmmp::Playing);
                mutex()->unlock();
                continue;
            }

            if(!m_decoders.isEmpty())
            {
                m_inputs.take(m_decoder)->deleteLater();
                delete m_decoder;
                m_decoder = m_decoders.dequeue();
                //m_seekTime = m_inputs.value(m_decoder)->offset();
                flush(true);
                //use current output if possible
                prepareEffects(m_decoder);
                if(m_ap == m_output->inputAudioParameters())
                {
                    StateHandler::instance()->sendFinished();
                    StateHandler::instance()->dispatch(Qmmp::Stopped); //fake stop/start cycle
                    StateHandler::instance()->dispatch(Qmmp::Buffering);
                    StateHandler::instance()->dispatch(m_decoder->totalTime());
                    m_output->seek(0); //reset counter
                    StateHandler::instance()->dispatch(Qmmp::Playing);
                    mutex()->unlock();
                    addOffset(); //offset
                }
                else
                {
                    finish();
                    //wake up waiting threads
                    mutex()->unlock();
                    m_output->recycler()->mutex()->lock();
                    m_output->recycler()->cond()->wakeAll();
                    m_output->recycler()->mutex()->unlock();

                    m_output->wait();
                    delete m_output;
                    m_output = createOutput();
                    m_dithering->setFormats(m_decoder->audioParameters().format(),
                                            m_output->outputAudioParameters().format());
                    if(m_output)
                    {
                        m_output->start();
                        StateHandler::instance()->dispatch(Qmmp::Playing);
                        StateHandler::instance()->dispatch(m_decoder->totalTime());
                        addOffset(); //offset
                    }
                }
                if(!m_output)
                    m_done = true;

                continue;
            }

            if(m_output)
            {
                flush(true);
                m_output->recycler()->mutex()->lock();
                // end of stream
                while(!m_output->recycler()->empty() && !m_user_stop)
                {
                    m_output->recycler()->cond()->wakeOne();
                    mutex()->unlock();
                    m_output->recycler()->cond()->wait(m_output->recycler()->mutex());
                    mutex()->lock();
                }
                m_output->recycler()->mutex()->unlock();
            }

            //continue if new input was queued
            if(m_decoders.isEmpty() || m_user_stop)
            {
                m_done = true;
                m_finish = !m_user_stop;
            }
        }
        else
            m_finish = true;
        mutex()->unlock();
    }
    clearDecoders();
    mutex()->lock();
    m_next = false;
    if(m_finish)
        finish();
    if(m_output)
        m_output->recycler()->cond()->wakeAll();

    mutex()->unlock();

    if(m_output)
    {
        if(m_user_stop || (m_done && !m_finish))
        {
            m_output->stop();
            m_output->recycler()->cond()->wakeAll();
        }

        if(m_output->isRunning())
        {
            m_output->wait();
        }
    }
}

void QmmpAudioEngine::flush(bool final)
{
    ulong min = final ? 0 : m_bks;

    while((!m_done && !m_finish) && m_output_at > min)
    {
        m_output->recycler()->mutex()->lock();

        while((m_output->recycler()->full() || m_output->recycler()->blocked()) && (!m_done && !m_finish))
        {
            if(m_seekTime > 0)
            {
                m_output_at = 0;
                m_output->recycler()->mutex()->unlock();
                return;
            }
            mutex()->unlock();
            m_output->recycler()->cond()->wait(m_output->recycler()->mutex());
            mutex()->lock();
            m_done = m_user_stop.load();
        }

        if(m_user_stop || m_finish)
            m_done = true;
        else
        {
            m_output_at -= produceSound(m_output_buf, m_output_at, m_bitrate);
        }

        if(!m_output->recycler()->empty())
        {
            m_output->recycler()->cond()->wakeOne();
        }

        m_output->recycler()->mutex()->unlock();
    }
}

void QmmpAudioEngine::addOffset()
{
    qint64 pos = m_inputs.value(m_decoder)->offset();
    if(pos > 0)
    {
        m_seekTime = pos;
        m_output->seek(pos);
    }
}

void QmmpAudioEngine::attachMetaData(Decoder *decoder, DecoderFactory *factory, InputSource *source)
{
    QString path = source->path();
    QString scheme = path.section("://", 0, 0);
    const QFileInfo fin(path);

    if(fin.isFile() || factory->properties().protocols.contains(scheme))
    {
        QStringList ignoredPaths;
        QList<TrackInfo*> playlist = factory->createPlayList(path, TrackInfo::AllParts, &ignoredPaths);
        if(!playlist.isEmpty())
        {
            TrackInfo *info = playlist.takeFirst();
            qDeleteAll(playlist);
            playlist.clear();
            decoder->addMetaData(info->metaData());
            if(info->parts() & TrackInfo::ReplayGainInfo)
                decoder->setReplayGainInfo(info->replayGainInfo());
            info->updateValues(decoder->properties());
            info->setValue(Qmmp::DECODER, factory->properties().shortName);
            if(fin.isFile() && info->value(Qmmp::FILE_SIZE).isEmpty())
                info->setValue(Qmmp::FILE_SIZE, fin.size());
            decoder->setProperties(info->properties());
            delete info;
        }
    }
    else
    {
        decoder->setProperty(Qmmp::DECODER, factory->properties().shortName);
        if(!decoder->hasMetaData())
            decoder->addMetaData(QMap<Qmmp::MetaData, QString>()); //add empty metadata to trigger track info update
    }
}

OutputWriter *QmmpAudioEngine::createOutput()
{
    OutputWriter *output = new OutputWriter(nullptr);
    if(!output->initialize(m_ap.sampleRate(), m_ap.channelMap()))
    {
        delete output;
        StateHandler::instance()->dispatch(Qmmp::FatalError);
        return nullptr;
    }
    return output;
}

void QmmpAudioEngine::prepareEffects(Decoder *d)
{
    m_ap = d->audioParameters();
    //output buffer for decoder
    if(m_output_buf)
        delete[] m_output_buf;
    m_bks = QMMP_BLOCK_FRAMES * m_ap.frameSize(); //block size
    m_output_size = m_bks * 4;
    m_sample_size = m_ap.sampleSize();
    m_output_buf = new unsigned char[m_output_size];
    //audio converter
    m_converter->configure(m_ap.format());
    m_ap = AudioParameters(m_ap.sampleRate(), m_ap.channelMap(), Qmmp::PCM_FLOAT);
    //remove disabled and external effects
    const QList<Effect *> tmp = m_effects;
    for(Effect *e : qAsConst(tmp))
    {
        if(!e->factory() || !Effect::isEnabled(e->factory()))
        {
            m_effects.removeAll(e);
            m_blockedEffects.removeAll(e);
        }
    }
    m_replayGain = nullptr;
    m_dithering = nullptr;
    QList<Effect *> tmp_effects = m_effects;
    m_effects.clear();

    //replay gain
    {
        m_replayGain = new ReplayGain();
        m_replayGain->configure(m_ap.sampleRate(), m_ap.channelMap());
        m_effects << m_replayGain;
        m_replayGain->updateSettings(m_settings->replayGainMode(),
                                     m_settings->replayGainPreamp(),
                                     m_settings->replayGainDefaultGain(),
                                     m_settings->replayGainPreventClipping());
        m_replayGain->setReplayGainInfo(d->replayGainInfo());
    }
    //dithering
    {
        m_dithering = new Dithering;
        m_dithering->configure(m_ap.sampleRate(), m_ap.channelMap());
        m_effects << m_dithering;
    }
    //channel order converter
    if(m_ap.channelMap() != m_ap.channelMap().remaped())
    {
        m_effects << new ChannelConverter(m_ap.channelMap().remaped());
        m_effects.back()->configure(m_ap.sampleRate(), m_ap.channelMap());
        m_ap = m_effects.back()->audioParameters();
    }

    for(EffectFactory *factory : Effect::enabledFactories())
    {
        Effect *effect = nullptr;
        for(Effect *e : qAsConst(tmp_effects)) //find effect
        {
            if(e->factory() == factory)
            {
                effect = e;
                break;
            }
        }

        if(effect && (effect->audioParameters() != m_ap ||
                      m_blockedEffects.contains(effect))) //destroy effect which require restart
        {
            m_blockedEffects.removeAll(effect);
            tmp_effects.removeAll(effect);
            delete effect;
            effect = nullptr;
        }
        if(!effect)
        {
            effect = Effect::create(factory);
            effect->configure(m_ap.sampleRate(), m_ap.channelMap());
            if(m_ap != effect->audioParameters())
            {
                m_blockedEffects << effect; //list of effects which require restart
                m_ap = effect->audioParameters();
            }
        }
        m_effects << effect;
        tmp_effects.removeAll(effect);
    }
}

//static members
QmmpAudioEngine *QmmpAudioEngine::m_instance = nullptr;

QmmpAudioEngine *QmmpAudioEngine::instance()
{
    return m_instance;
}
