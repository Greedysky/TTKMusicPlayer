/***************************************************************************
 *   Copyright (C) 2008-2016 by Ilya Kotov                                 *
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

#include <QTimer>
#include <QDir>
#include <QSettings>
#include "qmmpsettings.h"
#include "output.h"
#include "volumecontrol_p.h"

VolumeControl::VolumeControl(QObject *parent)
        : QObject(parent)
{
    m_left = 0;
    m_right = 0;
    m_prev_block = false;
    m_volume = 0;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(checkVolume()));
    reload();
}

VolumeControl::~VolumeControl()
{
    if(m_volume)
        delete m_volume;
}

void VolumeControl::setVolume(int left, int right)
{
    VolumeSettings v;
    v.left = qBound(0,left,100);
    v.right = qBound(0,right,100);
    m_volume->setVolume(v);
    checkVolume();
}

void VolumeControl::changeVolume(int delta)
{
    setVolume(qBound(0, volume() + delta, 100));
}

void VolumeControl::setVolume(int volume)
{
    volume = qBound(0, volume, 100);
    setVolume(volume-qMax(balance(),0)*volume/100,
              volume+qMin(balance(),0)*volume/100);
}

void VolumeControl::setBalance(int balance)
{
    balance = qBound(-100, balance, 100);
    setVolume(volume()-qMax(balance,0)*volume()/100,
              volume()+qMin(balance,0)*volume()/100);
}

int VolumeControl::left() const
{
    return m_left;
}

int VolumeControl::right() const
{
    return m_right;
}

int VolumeControl::volume() const
{
    return qMax(m_right, m_left);
}

int VolumeControl::balance() const
{
    int v = volume();
    return v > 0 ? (m_right - m_left)*100/v : 0;
}

void VolumeControl::checkVolume()
{
    VolumeSettings v = m_volume->volume();
    int l = v.left;
    int r = v.right;

    l = (l > 100) ? 100 : l;
    r = (r > 100) ? 100 : r;
    l = (l < 0) ? 0 : l;
    r = (r < 0) ? 0 : r;
    if (m_left != l || m_right != r) //volume has been changed
    {
        m_left = l;
        m_right = r;
        emit volumeChanged(m_left, m_right);
        emit volumeChanged(volume());
        emit balanceChanged(balance());
    }
    else if(m_prev_block && !signalsBlocked ()) //signals have been unblocked
    {
        emit volumeChanged(m_left, m_right);
        emit volumeChanged(volume());
        emit balanceChanged(balance());
    }
    m_prev_block = signalsBlocked ();
}

void VolumeControl::reload()
{
    m_timer->stop();
    if(m_volume)
    {
        delete m_volume;
        m_volume = 0;
    }
    if(!QmmpSettings::instance()->useSoftVolume() && Output::currentFactory())
    {
        if((m_volume = Output::currentFactory()->createVolume()))
        {
            if(m_volume->hasNotifySignal())
            {
                checkVolume();
                connect(m_volume, SIGNAL(changed()), SLOT(checkVolume()));
            }
            else
                m_timer->start(150); // fallback to polling if change notification is not available.
        }
    }
    if(!m_volume)
    {
        m_volume = new SoftwareVolume;
        blockSignals(true);
        checkVolume();
        blockSignals(false);
        QTimer::singleShot(125, this, SLOT(checkVolume()));
    }
}

SoftwareVolume *SoftwareVolume::m_instance = 0;

SoftwareVolume::SoftwareVolume()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_left = settings.value("Volume/left", 80).toInt();
    m_right = settings.value("Volume/right", 80).toInt();
    m_scaleLeft = (double)m_left/100.0;
    m_scaleRight = (double)m_right/100.0;
    m_instance = this;
}

SoftwareVolume::~SoftwareVolume()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Volume/left", m_left);
    settings.setValue("Volume/right", m_right);
    m_instance = 0;
}

void SoftwareVolume::setVolume(const VolumeSettings &v)
{
    m_left = v.left;
    m_right = v.right;
    m_scaleLeft = (double)m_left/100.0;
    m_scaleRight = (double)m_right/100.0;
}

VolumeSettings SoftwareVolume::volume() const
{
    VolumeSettings v;
    v.left = m_left;
    v.right = m_right;
    return v;
}

void SoftwareVolume::changeVolume(Buffer *b, int chan)
{
    if(chan == 1)
    {
        for(size_t i = 0; i < b->samples; ++i)
        {
            b->data[i] *= qMax(m_scaleLeft, m_scaleRight);
        }
    }
    else
    {
        for(size_t i = 0; i < b->samples; i+=2)
        {
            b->data[i] *= m_scaleLeft;
            b->data[i+1] *= m_scaleRight;
        }
    }
}

//static
SoftwareVolume *SoftwareVolume::instance()
{
    return m_instance;
}
