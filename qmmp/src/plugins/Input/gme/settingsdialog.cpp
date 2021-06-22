#include <QSettings>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

#ifndef Q_OS_LINUX
    setMinimumHeight(98);
    setMaximumHeight(98);
#endif

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.fadeoutCheckBox->setChecked(settings.value("GME/fadeout", false).toBool());
    m_ui.fadeoutSpinBox->setValue(settings.value("GME/fadeout_length", 7000).toInt());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("GME/fadeout", m_ui.fadeoutCheckBox->isChecked());
    settings.setValue("GME/fadeout_length", m_ui.fadeoutSpinBox->value());
    QDialog::accept();
}
