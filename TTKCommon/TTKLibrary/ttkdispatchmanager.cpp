#include "ttkdispatchmanager.h"

#include <QFile>
#include <QTimer>

TTKDispatchManager::TTKDispatchManager()
    : QObject(nullptr)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(2 * TTK_DN_S2MS);

    connect(m_timer, SIGNAL(timeout()), SLOT(activeFunctions()));
    m_timer->start();
}

TTKDispatchManager::~TTKDispatchManager()
{
    m_timer->stop();
    delete m_timer;
    qDeleteAll(m_observer);
}

void TTKDispatchManager::setInterval(int msec)
{
    m_timer->setInterval(msec);
}

void TTKDispatchManager::dispatch(Module type)
{
    Data *data = new Data;
    data->m_type = type;
    m_observer << data;
}

void TTKDispatchManager::dispatch(Module type, const TTKVariantList &args)
{
    Data *data = new Data;
    data->m_type = type;
    data->m_args << args;
    m_observer << data;
}

void TTKDispatchManager::dispatch(Module type, void *funcs)
{
    Data *data = new Data;
    data->m_type = type;
    data->m_args << QVariant::fromValue<void*>(funcs);
    m_observer << data;
}

void TTKDispatchManager::dispatch(Module type, const QVariant &arg1)
{
    Data *data = new Data;
    data->m_type = type;
    data->m_args << arg1;
    m_observer << data;
}

void TTKDispatchManager::dispatch(Module type, const QVariant &arg1, const QVariant &arg2)
{
    Data *data = new Data;
    data->m_type = type;
    data->m_args << arg1 << arg2;
    m_observer << data;
}

void TTKDispatchManager::dispatch(Module type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3)
{
    Data *data = new Data;
    data->m_type = type;
    data->m_args << arg1 << arg2 << arg3;
    m_observer << data;
}

void TTKDispatchManager::dispatch(Module type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3, const QVariant &arg4)
{
    Data *data = new Data;
    data->m_type = type;
    data->m_args << arg1 << arg2 << arg3 << arg4;
    m_observer << data;
}

void TTKDispatchManager::activeFunctions()
{
    m_mutex.lock();
    if(m_observer.isEmpty())
    {
        m_mutex.unlock();
        return;
    }

    Data *data = m_observer.takeLast();
    bool state = true;
    switch(data->m_type)
    {
        case Module::FileRemove:
        {
            if(!data->m_args.isEmpty())
            {
                const QString &path = data->m_args.first().toString();
                state = path.isEmpty() ? false : QFile::remove(path);
            }
            break;
        }
        default: break;
    }

    if(state)
    {
        delete data;
    }
    else
    {
        data->m_times++;
        if(data->isValid())
        {
            m_observer << data;
        }
        else
        {
            delete data;
        }
    }
    m_mutex.unlock();
}
