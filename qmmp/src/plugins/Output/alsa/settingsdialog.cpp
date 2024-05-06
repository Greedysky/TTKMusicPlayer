#include <QSettings>
#include <qmmp/qmmp.h>

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
#ifdef Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    m_ui.deviceComboBox->setEditable(true);

    getCards();
    getSoftDevices();
    connect(m_ui.deviceComboBox, SIGNAL(activated(int)), SLOT(setText(int)));
    connect(m_ui.mixerCardComboBox, SIGNAL(activated(int)), SLOT(showMixerDevices(int)));
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ALSA");
    m_ui.deviceComboBox->setEditText(settings.value("device", "default").toString());
    m_ui.bufferSpinBox->setValue(settings.value("buffer_time", 500).toInt());
    m_ui.periodSpinBox->setValue(settings.value("period_time", 100).toInt());

    int d = m_cards.indexOf(settings.value("mixer_card","hw:0").toString());
    if(d >= 0)
        m_ui.mixerCardComboBox->setCurrentIndex(d);

    showMixerDevices(m_ui.mixerCardComboBox->currentIndex());
    d = m_ui.mixerDeviceComboBox->findText(settings.value("mixer_device", "PCM").toString());

    if(d >= 0)
        m_ui.mixerDeviceComboBox->setCurrentIndex(d);

    m_ui.mmapCheckBox->setChecked(settings.value("use_mmap", false).toBool());
    m_ui.pauseCheckBox->setChecked(settings.value("use_snd_pcm_pause", false).toBool());
    settings.endGroup();
}

void SettingsDialog::getCards()
{
    int card = -1, err;

    m_devices.clear();
    m_devices << "default";
    m_ui.deviceComboBox->addItem("Default PCM device(default)");

    if((err = snd_card_next(&card)) !=0)
        qWarning("SettingsDialog(ALSA): snd_next_card() failed: %s", snd_strerror(-err));

    while(card > -1)
    {
        getCardDevices(card);
        m_cards << QString("hw:%1").arg(card);
        if((err = snd_card_next(&card)) !=0)
        {
            qWarning("SettingsDialog(ALSA): snd_next_card() failed: %s", snd_strerror(-err));
            break;
        }
    }
}

void SettingsDialog::getSoftDevices()
{
    void **hints = nullptr;
    int i = 0;

    if(snd_device_name_hint(-1, "pcm", &hints) < 0)
        return;

    while(hints && hints[i])
    {
        char *type = snd_device_name_get_hint(hints[i], "IOID");
        if(!type || !strcmp(type, "Output"))
        {
            char *device_name = snd_device_name_get_hint(hints[i], "NAME");
            char *device_desc = snd_device_name_get_hint(hints[i], "DESC");

            m_devices << QString(device_name);
            m_ui.deviceComboBox->addItem(QString("%1(%2)").arg(device_desc, device_name));
            free(device_name);
            free(device_desc);
        }
        if(type)
            free(type);
        ++i;
    }

    if(hints)
        snd_device_name_free_hint(hints);
}

void SettingsDialog::getCardDevices(int card)
{
    int pcm_device = -1, err;
    snd_pcm_info_t *pcm_info;
    snd_ctl_t *ctl;
    char dev[64], *card_name;

    sprintf(dev, "hw:%i", card);

    if((err = snd_ctl_open(&ctl, dev, 0)) < 0)
    {
        qWarning("SettingsDialog(ALSA): snd_ctl_open() failed: %s",
                 snd_strerror(-err));
        return;
    }

    if((err = snd_card_get_name(card, &card_name)) != 0)
    {
        qWarning("SettingsDialog(ALSA): snd_card_get_name() failed: %s", snd_strerror(-err));
        card_name = strdup("Unknown soundcard");
    }
    m_ui.mixerCardComboBox->addItem(QString(card_name));

    snd_pcm_info_alloca(&pcm_info);
    for(;;)
    {
        QString device;
        if((err = snd_ctl_pcm_next_device(ctl, &pcm_device)) < 0)
        {
            qWarning("SettingsDialog(ALSA): snd_ctl_pcm_next_device() failed: %s", snd_strerror(-err));
            pcm_device = -1;
        }

        if(pcm_device < 0)
            break;

        snd_pcm_info_set_device(pcm_info, pcm_device);
        snd_pcm_info_set_subdevice(pcm_info, 0);
        snd_pcm_info_set_stream(pcm_info, SND_PCM_STREAM_PLAYBACK);

        if((err = snd_ctl_pcm_info(ctl, pcm_info)) < 0)
        {
            if(err != -ENOENT)
                qWarning("SettingsDialog(ALSA): get_devices_for_card(): "
                         "snd_ctl_pcm_info() "
                         "failed(%d:%d): %s.", card,
                         pcm_device, snd_strerror(-err));
        }
        device = QString("hw:%1,%2").arg(card).arg(pcm_device);
        m_devices << device;
        m_ui.deviceComboBox->addItem(QString(card_name) + ": "+ snd_pcm_info_get_name(pcm_info) + "(" + device + ")");
    }

    free(card_name);
    snd_ctl_close(ctl);
}

void SettingsDialog::getMixerDevices(QString card)
{
    m_ui.mixerDeviceComboBox->clear();
    snd_mixer_t *mixer;
    snd_mixer_elem_t *current;

    if(getMixer(&mixer, card) < 0)
        return;

    current = snd_mixer_first_elem(mixer);

    while(current)
    {
        const char *sname = snd_mixer_selem_get_name(current);
        if(snd_mixer_selem_is_active(current) &&
                snd_mixer_selem_has_playback_volume(current))
            m_ui.mixerDeviceComboBox->addItem(QString(sname));
        current = snd_mixer_elem_next(current);
    }
}

void SettingsDialog::setText(int n)
{
    m_ui.deviceComboBox->setEditText(m_devices.at(n));
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ALSA");
    settings.setValue("device", m_ui.deviceComboBox->currentText());
    settings.setValue("buffer_time", m_ui.bufferSpinBox->value());
    settings.setValue("period_time", m_ui.periodSpinBox->value());

    if(m_ui.mixerCardComboBox->currentIndex() >= 0)
    {
        QString card = m_cards.at(m_ui.mixerCardComboBox->currentIndex());
        settings.setValue("mixer_card", card);
    }

    settings.setValue("mixer_device", m_ui.mixerDeviceComboBox->currentText());
    settings.setValue("use_mmap", m_ui.mmapCheckBox->isChecked());
    settings.setValue("use_snd_pcm_pause", m_ui.pauseCheckBox->isChecked());
    settings.endGroup();
    QDialog::accept();
}

int SettingsDialog::getMixer(snd_mixer_t **mixer, QString card)
{
    int err;

    if((err = snd_mixer_open(mixer, 0)) < 0)
    {
        qWarning("SettingsDialog(ALSA): alsa_get_mixer(): "
                 "Failed to open empty mixer: %s", snd_strerror(-err));
        mixer = nullptr;
        return -1;
    }
    if((err = snd_mixer_attach(*mixer, card.toLatin1().constData())) < 0)
    {
        qWarning("SettingsDialog(ALSA): alsa_get_mixer(): "
                 "Attaching to mixer %s failed: %s", qPrintable(card), snd_strerror(-err));
        return -1;
    }
    if((err = snd_mixer_selem_register(*mixer, nullptr, nullptr)) < 0)
    {
        qWarning("SettingsDialog(ALSA): alsa_get_mixer(): "
                 "Failed to register mixer: %s", snd_strerror(-err));
        return -1;
    }
    if((err = snd_mixer_load(*mixer)) < 0)
    {
        qWarning("SettingsDialog(ALSA): alsa_get_mixer(): Failed to load mixer: %s",
                 snd_strerror(-err));
        return -1;
    }

    return(*mixer != nullptr);
}

void SettingsDialog::showMixerDevices(int d)
{
    if(0<=d && d<m_cards.count())
        getMixerDevices(m_cards.at(d));
}
