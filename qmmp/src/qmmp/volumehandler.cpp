#include <QTimer>
#include <QDir>
#include <QSettings>
#include "qmmpsettings.h"
#include "output.h"
#include "volumehandler.h"

VolumeHandler *VolumeHandler::m_instance = nullptr;

VolumeHandler::VolumeHandler(QObject *parent) : QObject(parent)
{
    if(m_instance)
        qFatal("VolumeHandler: only one instance is allowed!");
    m_instance = this;

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_settings.left = settings.value("Volume/left", 80).toInt();
    m_settings.right = settings.value("Volume/right", 80).toInt();
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(checkVolume()));
    reload();
}

VolumeHandler::~VolumeHandler()
{
    m_instance = nullptr;
    delete m_volume;

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Volume/left", m_settings.left);
    settings.setValue("Volume/right", m_settings.right);
}

void VolumeHandler::setVolume(int left, int right)
{
    VolumeSettings v;
    v.left = qBound(0, left, 100);
    v.right = qBound(0, right, 100);
    if(m_volume)
    {
        m_volume->setVolume(v);
        checkVolume();
    }
    else if(m_settings != v)
    {
        m_settings = v;
        m_mutex.lock();
        m_scaleLeft = double(m_settings.left) / 100.0;
        m_scaleRight = double(m_settings.right) / 100.0;
        m_mutex.unlock();
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
    if(m_muted == muted)
        return;

    if(m_volume && (m_volume->flags() & Volume::IsMuteSupported))
    {
        m_volume->setMuted(muted);
        checkVolume();
    }
    else if(m_volume)
    {
        m_muted = muted;
        m_apply = muted;
        emit mutedChanged(muted);
    }
    else
    {
        m_muted = muted;
        emit mutedChanged(muted);
    }
}

int VolumeHandler::left() const
{
    return m_settings.left;
}

int VolumeHandler::right() const
{
    return m_settings.right;
}

int VolumeHandler::volume() const
{
    return qMax(m_settings.right, m_settings.left);
}

int VolumeHandler::balance() const
{
    int v = volume();
    return v > 0 ? (m_settings.right - m_settings.left) * 100 / v : 0;
}

bool VolumeHandler::isMuted() const
{
    return m_muted;
}

void VolumeHandler::apply(Buffer *b, int chan)
{
    if(m_apply)
    {
        if(m_muted)
        {
            memset(b->data, 0, b->samples * sizeof(float));
            return;
        }

        m_mutex.lock();
        if(chan == 1)
        {
            for(size_t i = 0; i < b->samples; ++i)
            {
                b->data[i] *= qMax(m_scaleLeft, m_scaleRight);
            }
        }
        else
        {
            for(size_t i = 0; i < b->samples; i += 2)
            {
                b->data[i] *= m_scaleLeft;
                b->data[i + 1] *= m_scaleRight;
            }
        }
        m_mutex.unlock();
    }
}

VolumeHandler *VolumeHandler::instance()
{
    return m_instance;
}

void VolumeHandler::checkVolume()
{
    if(!m_volume) //soft volume
    {
        emit volumeChanged(volume());
        emit balanceChanged(balance());
        return;
    }

    VolumeSettings v = m_volume->volume();
    const bool muted = m_volume->flags() & Volume::IsMuteSupported ? m_volume->isMuted() : isMuted();

    v.left = qBound(0, v.left, 100);
    v.right = qBound(0, v.right, 100);
    if(m_muted != muted || (m_prev_block && !signalsBlocked()))
    {
        m_muted = muted;
        emit mutedChanged(m_muted);
    }

    if(m_settings != v) //volume has been changed
    {
        m_settings = v;
        emit volumeChanged(volume());
        emit balanceChanged(balance());
    }
    else if(m_prev_block && !signalsBlocked()) //signals have been unblocked
    {
        emit volumeChanged(volume());
        emit balanceChanged(balance());
    }
    m_prev_block = signalsBlocked();
}

void VolumeHandler::reload()
{
    m_timer->stop();
    bool restore = false;
    if(m_volume)
    {
        restore = true;
        delete m_volume;
        m_volume = nullptr;
    }
    m_apply = false;

    if(!QmmpSettings::instance()->useSoftVolume() && Output::currentFactory())
        m_volume = Output::currentFactory()->createVolume();

    if(m_volume)
    {
        if(restore)
            m_volume->setMuted(m_muted);

        if(!(m_volume->flags() & Volume::IsMuteSupported) && m_muted)
            m_apply = true;

        if(m_volume->flags() & Volume::HasNotifySignal)
        {
            checkVolume();
            connect(m_volume, SIGNAL(changed()), SLOT(checkVolume()));
        }
        else
        {
            m_timer->start(150); // fallback to polling if change notification is not available.
        }
    }
    else
    {
        m_mutex.lock();
        m_scaleLeft = double(m_settings.left) / 100.0;
        m_scaleRight = double(m_settings.right) / 100.0;
        m_mutex.unlock();
        m_apply = true;
        blockSignals(true);
        checkVolume();
        blockSignals(false);
        QTimer::singleShot(125, this, SLOT(checkVolume()));
    }
}
