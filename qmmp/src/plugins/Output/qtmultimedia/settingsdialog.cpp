#include "settingsdialog.h"

#include <QSettings>
#include <qmmp/qmmp.h>
#include <QAbstractButton>
#include <QAudioDeviceInfo>

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

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    const QString &default_device = settings.value("QTMULTIMEDIA/device").toString();

	//Default item always has index = 0
    m_ui.deviceComboBox->addItem(tr("Default"));
    m_ui.deviceComboBox->setCurrentIndex(0);

    const QList<QAudioDeviceInfo> &devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
	int index = 1;
    for(const QAudioDeviceInfo &info : devices)
	{
        m_ui.deviceComboBox->addItem(info.deviceName());
        if(info.deviceName() == default_device)
        {
            m_ui.deviceComboBox->setCurrentIndex(index);
        }
		++index;
	}
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    //0 index means default value, we save empty string for it.
    settings.setValue("QTMULTIMEDIA/device", m_ui.deviceComboBox->currentIndex() ? m_ui.deviceComboBox->currentText() : QString());
    QDialog::accept();
}
