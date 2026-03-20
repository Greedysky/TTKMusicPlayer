#include <QTimer>
#include <atomic>
#include <QSettings>
#include "qmmpsettings.h"
#include "volume.h"
#include "output.h"
#include "volumehandler.h"

class VolumeHandlerPrivate
{
public:
    VolumeHandlerPrivate(VolumeHandler *q)
    {
        if(instance)
            qFatal("VolumeHandler: only one instance is allowed!");

        instance = q;

        const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        volumeSettings.left = settings.value("Volume/left", 80).toInt();
        volumeSettings.right = settings.value("Volume/right", 80).toInt();
        timer = new QTimer(q);
        QObject::connect(timer, SIGNAL(timeout()), q, SLOT(checkVolume()));
    }

    ~VolumeHandlerPrivate()
    {
        instance = nullptr;
        delete volume;

        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        settings.setValue("Volume/left", volumeSettings.left);
        settings.setValue("Volume/right", volumeSettings.right);
    }

    VolumeSettings volumeSettings;
    bool prevBlock = false;
    std::atomic_bool muted = ATOMIC_VAR_INIT(false);
    std::atomic_bool apply = ATOMIC_VAR_INIT(false);
    QMutex mutex;
    double scaleLeft = 0, scaleRight = 0;
    Volume *volume = nullptr;
    QTimer *timer;
    static VolumeHandler *instance;

};

VolumeHandler *VolumeHandlerPrivate::instance = nullptr;


VolumeHandler::VolumeHandler(QObject *parent)
    : QObject(parent),
      d(new VolumeHandlerPrivate(this))
{
    reload();
}

VolumeHandler::~VolumeHandler()
{
    delete d;
}

void VolumeHandler::setVolume(int left, int right)
{
    VolumeSettings v;
    v.left = qBound(0, left, 100);
    v.right = qBound(0, right, 100);
    if(d->volume)
    {
        d->volume->setVolume(v);
        checkVolume();
    }
    else if(d->volumeSettings != v)
    {
        d->volumeSettings = v;
        d->mutex.lock();
        d->scaleLeft = double(d->volumeSettings.left) / 100.0;
        d->scaleRight = double(d->volumeSettings.right) / 100.0;
        d->mutex.unlock();
        checkVolume();
    }
}

void VolumeHandler::changeVolume(int delta)
{
    setVolume(qBound(0, volume() + delta, 100));
}

void VolumeHandler::setVolume(int volume)
{
    volume = qBound(0, volume, 100);
    setVolume(volume - qMax(balance(), 0) * volume / 100,
              volume + qMin(balance(), 0) * volume / 100);
}

void VolumeHandler::setBalance(int balance)
{
    balance = qBound(-100, balance, 100);
    setVolume(volume() - qMax(balance, 0) * volume() / 100,
              volume() + qMin(balance, 0) * volume() / 100);
}

void VolumeHandler::setMuted(bool muted)
{
    if(d->muted == muted)
        return;

    if(d->volume && (d->volume->flags() & Volume::IsMuteSupported))
    {
        d->volume->setMuted(muted);
        checkVolume();
    }
    else if(d->volume)
    {
        d->muted = muted;
        d->apply = muted;
        emit mutedChanged(muted);
    }
    else
    {
        d->muted = muted;
        emit mutedChanged(muted);
    }
}

int VolumeHandler::left() const
{
    return d->volumeSettings.left;
}

int VolumeHandler::right() const
{
    return d->volumeSettings.right;
}

int VolumeHandler::volume() const
{
    return qMax(d->volumeSettings.right, d->volumeSettings.left);
}

int VolumeHandler::balance() const
{
    int v = volume();
    return v > 0 ? (d->volumeSettings.right - d->volumeSettings.left) * 100 / v : 0;
}

bool VolumeHandler::isMuted() const
{
    return d->muted;
}

void VolumeHandler::apply(Buffer *b, int chan)
{
    if(d->apply)
    {
        if(d->muted)
        {
            memset(b->data, 0, b->samples * sizeof(float));
            return;
        }

        d->mutex.lock();
        if(chan == 1)
        {
            for(size_t i = 0; i < b->samples; ++i)
            {
                b->data[i] *= qMax(d->scaleLeft, d->scaleRight);
            }
        }
        else
        {
            for(size_t i = 0; i < b->samples; i += 2)
            {
                b->data[i] *= d->scaleLeft;
                b->data[i + 1] *= d->scaleRight;
            }
        }
        d->mutex.unlock();
    }
}

VolumeHandler *VolumeHandler::instance()
{
    return VolumeHandlerPrivate::instance;
}

void VolumeHandler::checkVolume()
{
    if(!d->volume) //soft volume
    {
        emit volumeChanged(volume());
        emit balanceChanged(balance());
        return;
    }

    VolumeSettings v = d->volume->volume();
    const bool muted = d->volume->flags() & Volume::IsMuteSupported ? d->volume->isMuted() : isMuted();

    v.left = qBound(0, v.left, 100);
    v.right = qBound(0, v.right, 100);
    if(d->muted != muted || (d->prevBlock && !signalsBlocked()))
    {
        d->muted = muted;
        emit mutedChanged(d->muted);
    }

    if(d->volumeSettings != v) //volume has been changed
    {
        d->volumeSettings = v;
        emit volumeChanged(volume());
        emit balanceChanged(balance());
    }
    else if(d->prevBlock && !signalsBlocked()) //signals have been unblocked
    {
        emit volumeChanged(volume());
        emit balanceChanged(balance());
    }
    d->prevBlock = signalsBlocked();
}

void VolumeHandler::reload()
{
    d->timer->stop();
    bool restore = false;
    if(d->volume)
    {
        restore = true;
        delete d->volume;
        d->volume = nullptr;
    }
    d->apply = false;

    if(!QmmpSettings::instance()->useSoftVolume() && Output::currentFactory())
        d->volume = Output::currentFactory()->createVolume();

    if(d->volume)
    {
        if(restore)
            d->volume->setMuted(d->muted);

        if(!(d->volume->flags() & Volume::IsMuteSupported) && d->muted)
            d->apply = true;

        if(d->volume->flags() & Volume::HasNotifySignal)
        {
            checkVolume();
            connect(d->volume, SIGNAL(changed()), SLOT(checkVolume()));
        }
        else
        {
            d->timer->start(150); // fallback to polling if change notification is not available.
        }
    }
    else
    {
        d->mutex.lock();
        d->scaleLeft = double(d->volumeSettings.left) / 100.0;
        d->scaleRight = double(d->volumeSettings.right) / 100.0;
        d->mutex.unlock();
        d->apply = true;
        blockSignals(true);
        checkVolume();
        blockSignals(false);
        QTimer::singleShot(125, this, SLOT(checkVolume()));
    }
}
