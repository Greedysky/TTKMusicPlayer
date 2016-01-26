/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QMetaType>
#include <QIODevice>
#include <QFile>
#include "replaygain_p.h"
#include "effect.h"
#include "buffer.h"
#include "decoder.h"
#include "outputwriter_p.h"
#include "decoderfactory.h"
#include "effectfactory.h"
#include "inputsource.h"
#include "statehandler.h"
#include "audioconverter_p.h"
#include "qmmpaudioengine_p.h"
#include "channelconverter_p.h"
#include "metadatamanager.h"

#define TRANSPORT_TIMEOUT 5000 //ms

QmmpAudioEngine::QmmpAudioEngine(QObject *parent)
        : AbstractEngine(parent), m_factory(0), m_output(0)
{
    m_output_buf = 0;
    m_output_size = 0;
    m_bks = 0;
    m_decoder = 0;
    m_output = 0;
    m_muted = false;
    m_replayGain = new ReplayGain;
    m_settings = QmmpSettings::instance();
    connect(m_settings,SIGNAL(replayGainSettingsChanged()), SLOT(updateReplayGainSettings()));
    connect(m_settings, SIGNAL(eqSettingsChanged()), SLOT(updateEqSettings()));
    updateReplayGainSettings();
    reset();
    m_instance = this;
}

QmmpAudioEngine::~QmmpAudioEngine()
{
    stop();
    reset();
    if(m_output_buf)
        delete [] m_output_buf;
    m_output_buf = 0;
    qDeleteAll(m_effects);
    m_instance = 0;
    delete m_replayGain;
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
        m_inputs.take(m_decoder)->deleteLater ();
        delete m_decoder;
        m_decoder = 0;
    }
    while(!m_decoders.isEmpty())
    {
        Decoder *d = m_decoders.dequeue();
        m_inputs.take(d)->deleteLater ();
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
    if(m_decoder && m_decoder->nextURL() == source->url())
    {
        m_inputs.value(m_decoder)->setOffset(source->offset());
        delete source;
        m_next = true;
        mutex()->unlock();
        return true;
    }
    mutex()->unlock();

    DecoderFactory *factory = 0;

    if(!factory && !source->url().contains("://"))
        factory = Decoder::findByPath(source->url(), m_settings->determineFileTypeByContent());
    if(!factory)
        factory = Decoder::findByMime(source->contentType());
    if(!factory && source->ioDevice() && source->url().contains("://")) //ignore content of local files
        factory = Decoder::findByContent(source->ioDevice());
    if(!factory && source->url().contains("://"))
        factory = Decoder::findByProtocol(source->url().section("://",0,0));
    if(!factory)
    {
        qWarning("QmmpAudioEngine: unsupported file format");
        return false;
    }
    qDebug("QmmpAudioEngine: selected decoder: %s",qPrintable(factory->properties().shortName));
    if(factory->properties().noInput && source->ioDevice())
        source->ioDevice()->close();
    Decoder *decoder = factory->create(source->url(), source->ioDevice());
    if(!decoder->initialize())
    {
        qWarning("QmmpAudioEngine: invalid file format");
        delete decoder;
        return false;
    }
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
    foreach(Effect *effect, m_effects)
    {
        if(effect->factory() == factory)
        {
            qWarning("QmmpAudioEngine: effect %s already exists",
                     qPrintable(factory->properties().shortName));
            return;
        }
    }
    if(m_output && m_output->isRunning())
    {
        Effect *effect = Effect::create(factory);
        if(!effect)
            return;
        effect->configure(m_ap.sampleRate(), m_ap.channelMap(), m_ap.format());
        if(effect->audioParameters() == m_ap)
        {
            mutex()->lock();
            m_effects << effect;
            mutex()->unlock();
        }
        else
        {
            qDebug("QmmpAudioEngine: restart is required");
            delete effect;
        }
    }
}

void QmmpAudioEngine::removeEffect(EffectFactory *factory)
{
    Effect *effect = 0;
    foreach(Effect *e, m_effects)
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
    if (m_output && m_output->isRunning())
    {
        m_output->mutex()->lock ();
        m_output->seek(time, true);
        m_output->mutex()->unlock();
        if (isRunning())
        {
            mutex()->lock ();
            m_seekTime = time;
            mutex()->unlock();
        }
    }
}

void QmmpAudioEngine::pause()
{
    if (m_output)
        m_output->pause();

    if (m_output)
    {
        m_output->recycler()->mutex()->lock ();
        m_output->recycler()->cond()->wakeAll();
        m_output->recycler()->mutex()->unlock();
    }

}

void QmmpAudioEngine::setMuted(bool muted)
{
    m_muted = muted;
    if(m_output)
    {
        m_output->mutex()->lock();
        m_output->setMuted(muted);
        m_output->mutex()->unlock();
    }
}

void QmmpAudioEngine::stop()
{
    mutex()->lock ();
    m_user_stop = true;
    mutex()->unlock();

    if (m_output)
        m_output->recycler()->cond()->wakeAll();

    if(isRunning())
        wait();

    if (m_output)
    {
        m_output->mutex()->lock ();
        m_output->stop();
        m_output->mutex()->unlock();
    }

    // wake up threads
    if (m_output)
    {
        m_output->mutex()->lock();
        m_output->recycler()->cond()->wakeAll();
        m_output->mutex()->unlock();
        if(m_output->isRunning())
            m_output->wait();
        delete m_output;
        m_output = 0;
    }

    clearDecoders();
    reset();
    while(!m_effects.isEmpty()) //delete effects
        delete m_effects.takeFirst();
}

qint64 QmmpAudioEngine::produceSound(char *data, qint64 size, quint32 brate)
{
    Buffer *b = m_output->recycler()->get();
    uint sz = size < m_bks ? size : m_bks;
    //m_replayGain->applyReplayGain(data, sz);
    memcpy(b->data, data, sz);
    b->nbytes = sz;
    b->rate = brate;
    foreach(Effect* effect, m_effects)
    {
        effect->applyEffect(b);
    }
    size -= sz;
    memmove(data, data + sz, size);
    m_output->recycler()->add();
    return sz;
}

void QmmpAudioEngine::finish()
{
    if (m_output)
    {
        m_output->mutex()->lock ();
        m_output->finish();
        m_output->mutex()->unlock();
    }
    StateHandler::instance()->sendFinished();
}

void QmmpAudioEngine::updateReplayGainSettings()
{
    mutex()->lock();
    m_replayGain->updateSettings(m_settings->replayGainMode(),
                                 m_settings->replayGainPreamp(),
                                 m_settings->replayGainDefaultGain(),
                                 m_settings->replayGainPreventClipping());
    mutex()->unlock();
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
    mutex()->lock ();
    m_next = false;
    qint64 len = 0;
    int delay = 0;
    if(m_decoders.isEmpty())
    {
         mutex()->unlock ();
         return;
    }
    m_decoder = m_decoders.dequeue();
    addOffset(); //offset
    m_replayGain->setReplayGainInfo(m_decoder->replayGainInfo(), m_decoder->hasHeadroom());
    mutex()->unlock();
    m_output->start();
    StateHandler::instance()->dispatch(Qmmp::Buffering);
    StateHandler::instance()->dispatch(m_decoder->totalTime());
    StateHandler::instance()->dispatch(Qmmp::Playing);
    sendMetaData();

    while (!m_done && !m_finish)
    {
        mutex()->lock ();
        //seek
        if (m_seekTime >= 0)
        {
            m_decoder->seek(m_seekTime);
            m_seekTime = -1;
            m_output->recycler()->mutex()->lock ();
            m_output->recycler()->clear();
            m_output->recycler()->mutex()->unlock ();
            m_output_at = 0;
        }
        //metadata
        if(m_decoder->hasMetaData())
            StateHandler::instance()->dispatch(m_decoder->takeMetaData());
        if(m_inputs[m_decoder]->hasMetaData())
            StateHandler::instance()->dispatch(m_inputs[m_decoder]->takeMetaData());
        if(m_inputs[m_decoder]->hasStreamInfo())
            StateHandler::instance()->dispatch(m_inputs[m_decoder]->takeStreamInfo());
        //wait more data
        if(m_inputs[m_decoder]->isWaiting())
        {
            mutex()->unlock();
            msleep(5);
            delay += 5;
            mutex()->lock();
            m_done = m_user_stop;
            mutex()->unlock();
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
            len = m_replayGain->read(m_decoder,(char *)(m_output_buf + m_output_at), m_output_size - m_output_at);
            //len = m_decoder->read((char *)(m_output_buf + m_output_at), m_output_size - m_output_at);
        }

        if (len > 0)
        {
            m_bitrate = m_decoder->bitrate();
            m_output_at += len;
            flush();
        }
        else if (len == 0)
        {
            if(m_next) //decoder can play next track without initialization
            {
                m_next = false;
                qDebug("QmmpAudioEngine: switching to the next track");
                StateHandler::instance()->sendFinished();
                StateHandler::instance()->dispatch(Qmmp::Stopped); //fake stop/start cycle
                StateHandler::instance()->dispatch(Qmmp::Buffering);
                m_decoder->next();
                StateHandler::instance()->dispatch(m_decoder->totalTime());
                m_replayGain->setReplayGainInfo(m_decoder->replayGainInfo(), m_decoder->hasHeadroom());
                m_output->mutex()->lock();
                m_output->seek(0); //reset counter
                m_output->mutex()->unlock();
                addOffset(); //offset
                StateHandler::instance()->dispatch(Qmmp::Playing);
                mutex()->unlock();
                continue;
            }
            else if(!m_decoders.isEmpty())
            {
                m_inputs.take(m_decoder)->deleteLater ();
                delete m_decoder;
                m_decoder = m_decoders.dequeue();
                //m_seekTime = m_inputs.value(m_decoder)->offset();
                m_replayGain->setReplayGainInfo(m_decoder->replayGainInfo(), m_decoder->hasHeadroom());
                //use current output if possible
                prepareEffects(m_decoder);
                if(m_ap == m_output->audioParameters())
                {
                    StateHandler::instance()->sendFinished();
                    StateHandler::instance()->dispatch(Qmmp::Stopped); //fake stop/start cycle
                    StateHandler::instance()->dispatch(Qmmp::Buffering);
                    StateHandler::instance()->dispatch(m_decoder->totalTime());
                    m_output->mutex()->lock();
                    m_output->seek(0); //reset counter
                    m_output->mutex()->unlock();
                    StateHandler::instance()->dispatch(Qmmp::Playing);
                    mutex()->unlock();
                    sendMetaData();
                    addOffset(); //offset
                    continue;
                }
                else
                {
                    flush(true);
                    finish();
                    //wake up waiting threads
                    mutex()->unlock();
                    m_output->recycler()->mutex()->lock ();
                    m_output->recycler()->cond()->wakeAll();
                    m_output->recycler()->mutex()->unlock();

                    m_output->wait();
                    delete m_output;
                    m_output = createOutput();
                    if(m_output)
                    {
                        m_output->start();
                        StateHandler::instance()->dispatch(Qmmp::Playing);
                        StateHandler::instance()->dispatch(m_decoder->totalTime());
                        sendMetaData();
                        addOffset(); //offset
                        continue;
                    }
                }
            }

            flush(true);
            if (m_output)
            {
                m_output->recycler()->mutex()->lock ();
                // end of stream
                while (!m_output->recycler()->empty() && !m_user_stop)
                {
                    m_output->recycler()->cond()->wakeOne();
                    mutex()->unlock();
                    m_output->recycler()->cond()->wait(m_output->recycler()->mutex());
                    mutex()->lock ();
                }
                m_output->recycler()->mutex()->unlock();
            }
            m_done = true;
            m_finish = !m_user_stop;
        }
        else
            m_finish = true;
        mutex()->unlock();
    }
    clearDecoders();
    mutex()->lock ();
    m_next = false;
    if (m_finish)
        finish();
    if(m_output)
    {
        m_output->mutex()->lock();
        m_output->recycler()->cond()->wakeAll();
        m_output->mutex()->unlock();
    }
    mutex()->unlock();
}

void QmmpAudioEngine::flush(bool final)
{
    ulong min = final ? 0 : m_bks;

    while ((!m_done && !m_finish) && m_output_at > min)
    {
        m_output->recycler()->mutex()->lock ();

        while ((m_output->recycler()->full() || m_output->recycler()->blocked()) && (!m_done && !m_finish))
        {
            if(m_seekTime > 0)
            {
                m_output_at = 0;
                m_output->recycler()->mutex()->unlock ();
                return;
            }
            mutex()->unlock();
            m_output->recycler()->cond()->wait(m_output->recycler()->mutex());
            mutex()->lock ();
            m_done = m_user_stop;
        }

        if (m_user_stop || m_finish)
            m_done = true;
        else
        {
            m_output_at -= produceSound((char*)m_output_buf, m_output_at, m_bitrate);
        }

        if (!m_output->recycler()->empty())
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

void QmmpAudioEngine::sendMetaData()
{
    if(!m_decoder || m_inputs.isEmpty())
        return;
    QString url = m_inputs.value(m_decoder)->url();
    if (QFile::exists(url)) //send metadata for local files only
    {
        QList <FileInfo *> list = MetaDataManager::instance()->createPlayList(url, true);
        if (!list.isEmpty())
        {
            StateHandler::instance()->dispatch(list[0]->metaData());
            while (!list.isEmpty())
                delete list.takeFirst();
        }
    }
}

OutputWriter *QmmpAudioEngine::createOutput()
{
    OutputWriter *output = new OutputWriter(0);
    output->setMuted(m_muted);
    if (!output->initialize(m_ap.sampleRate(), m_ap.channelMap(), m_ap.format()))
    {
        delete output;
        StateHandler::instance()->dispatch(Qmmp::FatalError);
        return 0;
    }

    if(m_output_buf)
        delete [] m_output_buf;
    m_bks = output->recycler()->blockSize();
    m_output_size = m_bks * 4;
    m_output_buf = new unsigned char[m_output_size];
    return output;
}

void QmmpAudioEngine::prepareEffects(Decoder *d)
{
    m_ap = d->audioParameters();

    //m_ap = AudioParameters(44100, 2, Qmmp::PCM_S24LE);

    m_replayGain->configure(m_ap);

    foreach(Effect *e, m_effects) //remove disabled and external effects
    {
        if(!e->factory() || !Effect::isEnabled(e->factory()))
        {
            m_effects.removeAll(e);
            m_blockedEffects.removeAll(e);
            delete e;
        }
    }
    QList <Effect *> tmp_effects = m_effects;
    m_effects.clear();

    m_effects << new ChannelConverter(m_ap.channelMap().remaped());
    m_effects.at(0)->configure(m_ap.sampleRate(), m_ap.channelMap(), m_ap.format());
    m_ap = m_effects.at(0)->audioParameters();

    if(m_settings->use16BitOutput())
    {
        m_effects << new AudioConverter();
        m_effects.at(0)->configure(m_ap.sampleRate(), m_ap.channelMap(), m_ap.format());
        m_ap = m_effects.at(0)->audioParameters();
    }

    foreach(EffectFactory *factory, Effect::enabledFactories())
    {
        Effect *effect = 0;
        foreach(Effect *e, tmp_effects) //find effect
        {
            if(e->factory() == factory)
                effect = e;
        }

        if(effect && (effect->audioParameters() != m_ap ||
                      m_blockedEffects.contains(effect))) //destroy effect which require restart
        {
            m_blockedEffects.removeAll(effect);
            tmp_effects.removeAll(effect);
            delete effect;
            effect = 0;
        }
        if(!effect)
        {
            effect = Effect::create(factory);
            effect->configure(m_ap.sampleRate(), m_ap.channelMap(), m_ap.format());
            if (m_ap != effect->audioParameters())
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
QmmpAudioEngine *QmmpAudioEngine::m_instance = 0;

QmmpAudioEngine *QmmpAudioEngine::instance()
{
    return m_instance;
}
