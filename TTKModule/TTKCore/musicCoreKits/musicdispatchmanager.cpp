#include "musicdispatchmanager.h"

#include <QTimer>

MusicDispatchManager::MusicDispatchManager()
    : QObject(nullptr)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(5000);
    connect(m_timer, SIGNAL(timeout()), SLOT(activeFunctions()));
    m_timer->start();
}

MusicDispatchManager::~MusicDispatchManager()
{
    m_timer->stop();
    delete m_timer;
    qDeleteAll(m_observer);
}

void MusicDispatchManager::dispatch(int type)
{
    MusicDispatchItem *item = new MusicDispatchItem;
    item->m_type = type;
    m_observer << item;
}

void MusicDispatchManager::dispatch(int type, const QVariant &arg1)
{
    MusicDispatchItem *item = new MusicDispatchItem;
    item->m_type = type;
    item->m_args << arg1;
    m_observer << item;
}

void MusicDispatchManager::dispatch(int type, const QVariant &arg1, const QVariant &arg2)
{
    MusicDispatchItem *item = new MusicDispatchItem;
    item->m_type = type;
    item->m_args << arg1 << arg2;
    m_observer << item;
}

void MusicDispatchManager::dispatch(int type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3)
{
    MusicDispatchItem *item = new MusicDispatchItem;
    item->m_type = type;
    item->m_args << arg1 << arg2 << arg3;
    m_observer << item;
}

void MusicDispatchManager::dispatch(int type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3, const QVariant &arg4)
{
    MusicDispatchItem *item = new MusicDispatchItem;
    item->m_type = type;
    item->m_args << arg1 << arg2 << arg3 << arg4;
    m_observer << item;
}

void MusicDispatchManager::dispatch(int type, const QList<QVariant> &args)
{
    MusicDispatchItem *item = new MusicDispatchItem;
    item->m_type = type;
    item->m_args << args;
    m_observer << item;
}

void MusicDispatchManager::dispatch(int type, void *funcs)
{
    MusicDispatchItem *item = new MusicDispatchItem;
    item->m_type = type;
    item->m_args << QVariant::fromValue<void*>(funcs);
    m_observer << item;
}

void MusicDispatchManager::activeFunctions()
{
    m_mutex.lock();
    if(m_observer.isEmpty())
    {
        m_mutex.unlock();
        return;
    }

    MusicDispatchItem *item = m_observer.takeLast();
    bool state = true;
    switch(item->m_type)
    {
        case 0: break;
        case 1:
        {
            if(item->m_args.count() == 1)
            {
                state = QFile::remove(item->m_args.first().toString());
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
