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
    settings.beginGroup("NEZplug");
    m_ui.masterVolume->setValue(settings.value("gain", int32_t(255.0f * 2.0f / 7.0f)).toInt());
    m_ui.devicePan->setValue(settings.value("filter", 0).toInt());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NEZplug");
    settings.setValue("gain", m_ui.masterVolume->value());
    settings.setValue("filter", m_ui.devicePan->value());
    settings.endGroup();
    QDialog::accept();
}
