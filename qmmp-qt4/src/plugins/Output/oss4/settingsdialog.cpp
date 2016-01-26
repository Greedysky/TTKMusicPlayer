/***************************************************************************
 *   Copyright (C) 2010 by Ilya Kotov                                      *
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
#include <QSettings>
#include <QDir>
#include <qmmp/qmmp.h>

extern "C"
{
#ifdef HAVE_SYS_SOUNDCARD_H
#include <sys/soundcard.h>
#else
#include <soundcard.h>
#endif
//#include </usr/lib/oss/include/sys/soundcard.h>
}
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include "outputoss4.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog (QWidget *parent) : QDialog (parent)
{
    ui.setupUi (this);
    setAttribute (Qt::WA_DeleteOnClose);



    int mixer_fd = -1;
    oss_sysinfo info;
    if ((mixer_fd = ::open(DEFAULT_MIXER, O_RDWR)) < 0)
    {
        qWarning("SettingsDialog: %s", strerror(errno));
        return;
    }
    if (ioctl(mixer_fd, SNDCTL_SYSINFO, &info) < 0)
    {
        qWarning("SettingsDialog: ioctl SNDCTL_SYSINFO failed: %s", strerror(errno));
        return;
    }

    if (info.numaudios < 1)
    {
        qWarning("SettingsDialog: no device found");
        return;
    }

    m_devices << DEFAULT_DEV;
    ui.deviceComboBox->addItem(tr("Default") + " (" + DEFAULT_DEV + ")");

    for (int i = 0; i < info.numaudios; ++i)
    {
        oss_audioinfo audio_info;
        audio_info.dev = i;

        if (ioctl(mixer_fd, SNDCTL_AUDIOINFO, &audio_info) < 0)
        {
            qWarning("SettingsDialog: ioctl SNDCTL_AUDIOINFO failed: %s", strerror(errno));
            return;
        }

        if (audio_info.caps & PCM_CAP_OUTPUT)
        {
            m_devices << audio_info.devnode;
            ui.deviceComboBox->addItem(QString("%1 (%2)").arg(audio_info.name).arg(audio_info.devnode));
        }
    }
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    ui.deviceComboBox->setEditText(settings.value("OSS4/device", DEFAULT_DEV).toString());
    connect (ui.deviceComboBox, SIGNAL(activated(int)),SLOT(setText(int)));
}

SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::setText(int n)
{
    ui.deviceComboBox->setEditText(m_devices.at(n));
}

void SettingsDialog::accept()
{
    qDebug("%s", Q_FUNC_INFO);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("OSS4/device", ui.deviceComboBox->currentText ());
    QDialog::accept();
}
