#include "settingsdialog.h"

#include <QSettings>
#include <qmmp/qmmp.h>
#include <QAbstractButton>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#  include <QAudioDeviceInfo>
#else
#  include <QAudioDevice>
#  include <QMediaDevices>
#endif

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

    int index = 1;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QList<QAudioDeviceInfo> &devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for(const QAudioDeviceInfo &info : devices)
	{
        const QString &device_name = info.deviceName();
#else
    const QList<QAudioDevice> &devices = QMediaDevices::audioOutputs();
    for(const QAudioDevice &info : devices)
    {
        const QString &device_name = info.id();
#endif
        m_ui.deviceComboBox->addItem(device_name);
        if(device_name == default_device)
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
