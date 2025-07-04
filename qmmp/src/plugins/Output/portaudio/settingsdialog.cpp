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

    for(int i = 0; i < Pa_GetDeviceCount(); ++i)
    {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        m_ui.deviceComboBox->addItem(deviceInfo->name);
        if(deviceInfo->name == device_name)
        {
            m_ui.deviceComboBox->setCurrentIndex(i);
        }
    }

    Pa_Terminate();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("PORTAUDIO/buffer", m_ui.bufferSizeSpinBox->value());
    //0 index means default value, we save empty string for it.
    settings.setValue("PORTAUDIO/device", m_ui.deviceComboBox->currentIndex() ? m_ui.deviceComboBox->currentText() : "default");
    QDialog::accept();
}
