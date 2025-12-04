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
    settings.beginGroup("KSS");
    m_ui.masterVolume->setValue(settings.value("master_volume", 32).toInt());
    m_ui.devicePan->setValue(settings.value("device_pan", 32).toInt());
    m_ui.silence->setValue(settings.value("silence", 5).toInt());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("KSS");
    settings.setValue("master_volume", m_ui.masterVolume->value());
    settings.setValue("device_pan", m_ui.devicePan->value());
    settings.setValue("silence", m_ui.silence->value());
    settings.endGroup();
    QDialog::accept();
}
