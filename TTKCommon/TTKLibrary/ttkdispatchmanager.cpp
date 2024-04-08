#include "ttkdispatchmanager.h"

#include <QFile>
#include <QTimer>

TTKDispatchManager::TTKDispatchManager()
    : QObject(nullptr)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(5 * TTK_DN_S2MS);

    connect(m_timer, SIGNAL(timeout()), SLOT(activeFunctions()));
    m_timer->start();
}

TTKDispatchManager::~TTKDispatchManager()
{
    m_timer->stop();
    delete m_timer;
    qDeleteAll(m_observer);
}

void TTKDispatchManager::dispatch(Module type)
{
    TTKDispatchItem *item = new TTKDispatchItem;
    item->m_type = type;
    m_observer << item;
}

void TTKDispatchManager::dispatch(Module type, const TTKVariantList &args)
{
    TTKDispatchItem *item = new TTKDispatchItem;
    item->m_type = type;
    item->m_args << args;
    m_observer << item;
}

void TTKDispatchManager::dispatch(Module type, void *funcs)
{
    TTKDispatchItem *item = new TTKDispatchItem;
    item->m_type = type;
    item->m_args << QVariant::fromValue<void*>(funcs);
    m_observer << item;
}

void TTKDispatchManager::dispatch(Module type, const QVariant &arg1)
{
    TTKDispatchItem *item = new TTKDispatchItem;
    item->m_type = type;
    item->m_args << arg1;
    m_observer << item;
}

void TTKDispatchManager::dispatch(Module type, const QVariant &arg1, const QVariant &arg2)
{
    TTKDispatchItem *item = new TTKDispatchItem;
    item->m_type = type;
    item->m_args << arg1 << arg2;
    m_observer << item;
}

void TTKDispatchManager::dispatch(Module type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3)
{
    TTKDispatchItem *item = new TTKDispatchItem;
    item->m_type = type;
    item->m_args << arg1 << arg2 << arg3;
    m_observer << item;
}

void TTKDispatchManager::dispatch(Module type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3, const QVariant &arg4)
{
    TTKDispatchItem *item = new TTKDispatchItem;
    item->m_type = type;
    item->m_args << arg1 << arg2 << arg3 << arg4;
    m_observer << item;
}

void TTKDispatchManager::activeFunctions()
{
    m_mutex.lock();
    if(m_observer.isEmpty())
    {
        m_mutex.unlock();
        return;
    }

    TTKDispatchItem *item = m_observer.takeLast();
    bool state = true;
    switch(item->m_type)
    {
        case Module::FileRemove:
        {
            if(!item->m_args.isEmpty())
            {
                state = QFile::remove(item->m_args.front().toString());
            }
            break;
        }
        default: break;
    }

    if(state)
    {
        delete item;
    }
    else
    {
        item->m_times++;
        if(item->isValid())
        {
            m_observer << item;
        }
    }
    m_mutex.unlock();
}
