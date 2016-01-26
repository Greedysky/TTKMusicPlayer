/***************************************************************************
 *   Copyright (C) 2006-2012 by Ilya Kotov                                 *
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

#include <qmmp/qmmp.h>

extern "C"
{
#include <alsa/asoundlib.h>
}

#include "settingsdialog.h"

SettingsDialog::SettingsDialog ( QWidget *parent )
        : QDialog ( parent )
{
    ui.setupUi (this);
    setAttribute (Qt::WA_DeleteOnClose);
    ui.deviceComboBox->setEditable (true);
    getCards();
    getSoftDevices();
    connect (ui.deviceComboBox, SIGNAL(activated(int)),SLOT(setText(int)));
    connect(ui.mixerCardComboBox, SIGNAL(activated(int)), SLOT(showMixerDevices(int)));
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ALSA");
    ui.deviceComboBox->setEditText(settings.value("device","default").toString());
    ui.bufferSpinBox->setValue(settings.value("buffer_time",500).toInt());
    ui.periodSpinBox->setValue(settings.value("period_time",100).toInt());

    int d = m_cards.indexOf(settings.value("mixer_card","hw:0").toString());
    if (d >= 0)
        ui.mixerCardComboBox->setCurrentIndex(d);

    showMixerDevices(ui.mixerCardComboBox->currentIndex ());
    d = ui.mixerDeviceComboBox->findText(settings.value("mixer_device",
                                         "PCM").toString());

    if (d >= 0)
        ui.mixerDeviceComboBox->setCurrentIndex(d);

    ui.mmapCheckBox->setChecked(settings.value("use_mmap", false).toBool());
    ui.pauseCheckBox->setChecked(settings.value("use_snd_pcm_pause", false).toBool());
    settings.endGroup();
}


SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::getCards()
{
    int card = -1, err;

    m_devices.clear();
    m_devices << "default";
    ui.deviceComboBox->addItem("Default PCM device (default)");

    if ((err = snd_card_next(&card)) !=0)
        qWarning("SettingsDialog (ALSA): snd_next_card() failed: %s",
                 snd_strerror(-err));

    while (card > -1)
    {
        getCardDevices(card);
        m_cards << QString("hw:%1").arg(card);
        if ((err = snd_card_next(&card)) !=0)
        {
            qWarning("SettingsDialog (ALSA): snd_next_card() failed: %s",
                     snd_strerror(-err));
            break;
        }
    }
}

void SettingsDialog::getSoftDevices()
{
#if SND_LIB_VERSION >= 0x010016 //1.0.22
    void **hints = 0;
    int i = 0;

    if(snd_device_name_hint(-1, "pcm", &hints) < 0)
        return;

    while(hints[i])
    {
        char *type = snd_device_name_get_hint (hints[i], "IOID");
        if (!type || !strcmp (type, "Output"))
        {
            char *device_name = snd_device_name_get_hint (hints[i], "NAME");
            char *device_desc = snd_device_name_get_hint (hints[i], "DESC");

            m_devices << QString(device_name);
            QString str = QString("%1 (%2)").arg(device_desc).arg(device_name);
            qDebug("%s", qPrintable(str));
            ui.deviceComboBox->addItem(str);
            free (device_name);
            free (device_desc);
        }
        if(type)
            free (type);
        ++i;
    }

    if (hints)
        snd_device_name_free_hint (hints);
#endif
}

void SettingsDialog::getCardDevices(int card)
{
    int pcm_device = -1, err;
    snd_pcm_info_t *pcm_info;
    snd_ctl_t *ctl;
    char dev[64], *card_name;

    sprintf(dev, "hw:%i", card);

    if ((err = snd_ctl_open(&ctl, dev, 0)) < 0)
    {
        qWarning("SettingsDialog (ALSA): snd_ctl_open() failed: %s",
                 snd_strerror(-err));
        return;
    }

    if ((err = snd_card_get_name(card, &card_name)) != 0)
    {
        qWarning("SettingsDialog (ALSA): snd_card_get_name() failed: %s",
                 snd_strerror(-err));
        card_name = strdup("Unknown soundcard");
    }
    ui.mixerCardComboBox->addItem(QString(card_name));

    snd_pcm_info_alloca(&pcm_info);

    qDebug("SettingsDialog (ALSA): detected sound cards:");

    for (;;)
    {
        QString device;
        if ((err = snd_ctl_pcm_next_device(ctl, &pcm_device)) < 0)
        {
            qWarning("SettingsDialog (ALSA): snd_ctl_pcm_next_device() failed: %s",
                     snd_strerror(-err));
            pcm_device = -1;
        }
        if (pcm_device < 0)
            break;

        snd_pcm_info_set_device(pcm_info, pcm_device);
        snd_pcm_info_set_subdevice(pcm_info, 0);
        snd_pcm_info_set_stream(pcm_info, SND_PCM_STREAM_PLAYBACK);

        if ((err = snd_ctl_pcm_info(ctl, pcm_info)) < 0)
        {
            if (err != -ENOENT)
                qWarning("SettingsDialog (ALSA): get_devices_for_card(): "
                         "snd_ctl_pcm_info() "
                         "failed (%d:%d): %s.", card,
                         pcm_device, snd_strerror(-err));
        }
        device = QString("hw:%1,%2").arg(card).arg(pcm_device);
        m_devices << device;
        QString str;
        str =  QString(card_name) + ": "+
               snd_pcm_info_get_name(pcm_info)+" ("+device+")";
        qDebug("%s",qPrintable(str));
        ui.deviceComboBox->addItem(str);
    }

    snd_ctl_close(ctl);
}

void SettingsDialog::getMixerDevices(QString card)
{
    ui.mixerDeviceComboBox->clear();
    int err;
    snd_mixer_t *mixer;
    snd_mixer_elem_t *current;

    if ((err = getMixer(&mixer, card)) < 0)
        return;

    current = snd_mixer_first_elem(mixer);

    while (current)
    {
        const char *sname = snd_mixer_selem_get_name(current);
        if (snd_mixer_selem_is_active(current) &&
                snd_mixer_selem_has_playback_volume(current))
            ui.mixerDeviceComboBox->addItem(QString(sname));
        current = snd_mixer_elem_next(current);
    }
}

void SettingsDialog::setText(int n)
{
    ui.deviceComboBox->setEditText(m_devices.at(n));
}

void SettingsDialog::accept()
{
    qDebug("SettingsDialog (ALSA):: writeSettings()");
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ALSA");
    settings.setValue("device", ui.deviceComboBox->currentText ());
    settings.setValue("buffer_time",ui.bufferSpinBox->value());
    settings.setValue("period_time",ui.periodSpinBox->value());
    if(ui.mixerCardComboBox->currentIndex() >= 0)
    {
        QString card = m_cards.at(ui.mixerCardComboBox->currentIndex());
        settings.setValue("mixer_card", card);
    }
    settings.setValue("mixer_device", ui.mixerDeviceComboBox->currentText ());
    settings.setValue("use_mmap", ui.mmapCheckBox->isChecked());
    settings.setValue("use_snd_pcm_pause", ui.pauseCheckBox->isChecked());
    settings.endGroup();
    QDialog::accept();
}

int SettingsDialog::getMixer(snd_mixer_t **mixer, QString card)
{
    char *dev;
    int err;

    dev = strdup(QString(card).toAscii().data());
    if ((err = snd_mixer_open(mixer, 0)) < 0)
    {
        qWarning("SettingsDialog (ALSA): alsa_get_mixer(): "
                 "Failed to open empty mixer: %s", snd_strerror(-err));
        mixer = NULL;
        return -1;
    }
    if ((err = snd_mixer_attach(*mixer, dev)) < 0)
    {
        qWarning("SettingsDialog (ALSA): alsa_get_mixer(): "
                 "Attaching to mixer %s failed: %s", dev, snd_strerror(-err));
        return -1;
    }
    if ((err = snd_mixer_selem_register(*mixer, NULL, NULL)) < 0)
    {
        qWarning("SettingsDialog (ALSA): alsa_get_mixer(): "
                 "Failed to register mixer: %s", snd_strerror(-err));
        return -1;
    }
    if ((err = snd_mixer_load(*mixer)) < 0)
    {
        qWarning("SettingsDialog (ALSA): alsa_get_mixer(): Failed to load mixer: %s",
                 snd_strerror(-err));
        return -1;
    }

    free (dev);

    return (*mixer != NULL);
}

void SettingsDialog::showMixerDevices(int d)
{
    if (0<=d && d<m_cards.size())
        getMixerDevices(m_cards.at(d));
}
