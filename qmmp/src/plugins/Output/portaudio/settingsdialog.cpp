#include "settingsdialog.h"

extern "C"{
#include <portaudio.h>
}

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

    PaError err = Pa_Initialize();
    if(err != paNoError)
    {
        return;
    }

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    const QString &device_name = settings.value("PORTAUDIO/device", "default").toString();
    const int buffer = settings.value("PORTAUDIO/buffer", paFramesPerBufferUnspecified).toInt();
    m_ui.bufferSizeSpinBox->setValue(buffer);

    for(int index = 0; index < Pa_GetDeviceCount(); ++index)
    {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(index);
        m_ui.deviceComboBox->addItem(deviceInfo->name);
        if(deviceInfo->name == device_name)
        {
            m_ui.deviceComboBox->setCurrentIndex(index);
        }
    }

    Pa_Terminate();

    if(m_ui.deviceComboBox->count() != 0 && m_ui.deviceComboBox->currentIndex() == -1)
    {
        m_ui.deviceComboBox->setCurrentIndex(0); // choose first one
    }
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("PORTAUDIO/buffer", m_ui.bufferSizeSpinBox->value());
    settings.setValue("PORTAUDIO/device", m_ui.deviceComboBox->currentText());
    QDialog::accept();
}
