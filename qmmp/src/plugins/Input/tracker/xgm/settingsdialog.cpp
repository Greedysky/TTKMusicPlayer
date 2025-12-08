#include "settingsdialog.h"

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
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("XGM");
    m_ui.kssMasterVolume->setValue(settings.value("kss_master_volume", 32).toInt());
    m_ui.kssDevicePan->setValue(settings.value("kss_device_pan", 32).toInt());
    m_ui.kssSilence->setValue(settings.value("kss_silence", 5).toInt());
    m_ui.nezFilter->setCurrentIndex(settings.value("nez_filter", 0).toInt());
    m_ui.nezGain->setValue(settings.value("nez_gain", 3).toInt());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("XGM");
    settings.setValue("kss_master_volume", m_ui.kssMasterVolume->value());
    settings.setValue("kss_device_pan", m_ui.kssDevicePan->value());
    settings.setValue("kss_silence", m_ui.kssSilence->value());
    settings.setValue("nez_filter", m_ui.nezFilter->currentIndex());
    settings.setValue("nez_gain", m_ui.nezGain->value());
    settings.endGroup();
    QDialog::accept();
}
