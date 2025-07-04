#include "settingsdialog.h"
#include "outputoss4.h"

#include <QSettings>
#include <qmmp/qmmp.h>
#include <QAbstractButton>

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

    int mixer_fd = -1;
    oss_sysinfo info;
    if((mixer_fd = ::open(DEFAULT_MIXER, O_RDWR)) < 0)
    {
        qWarning("SettingsDialog: %s %s", DEFAULT_MIXER, strerror(errno));
        return;
    }

    if(ioctl(mixer_fd, SNDCTL_SYSINFO, &info) < 0)
    {
        qWarning("SettingsDialog: ioctl SNDCTL_SYSINFO failed: %s", strerror(errno));
        return;
    }

    if(info.numaudios < 1)
    {
        qWarning("SettingsDialog: no device found");
        return;
    }

    m_devices << DEFAULT_DEV;
    m_ui.deviceComboBox->addItem(tr("Default") + " (" + DEFAULT_DEV + ")");

    for(int i = 0; i < info.numaudios; ++i)
    {
        oss_audioinfo audio_info;
        audio_info.dev = i;

        if(ioctl(mixer_fd, SNDCTL_AUDIOINFO, &audio_info) < 0)
        {
            qWarning("SettingsDialog: ioctl SNDCTL_AUDIOINFO failed: %s", strerror(errno));
            return;
        }

        if(audio_info.caps & PCM_CAP_OUTPUT)
        {
            m_devices << audio_info.devnode;
            m_ui.deviceComboBox->addItem(QString("%1 (%2)").arg(audio_info.name, audio_info.devnode));
        }
    }

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.deviceComboBox->setEditText(settings.value("OSS4/device", DEFAULT_DEV).toString());
    connect(m_ui.deviceComboBox, SIGNAL(activated(int)), SLOT(setText(int)));
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("OSS4/device", m_ui.deviceComboBox->currentText());
    QDialog::accept();
}

void SettingsDialog::setText(int n)
{
    m_ui.deviceComboBox->setEditText(m_devices.at(n));
}
